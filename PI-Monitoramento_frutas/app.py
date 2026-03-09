from flask import Flask, request, jsonify, render_template, send_from_directory
from flask_socketio import SocketIO
from pymongo import MongoClient
from datetime import datetime, timedelta, timezone
import pandas as pd
import os
import joblib
import csv
from sklearn.linear_model import LinearRegression
import numpy as np
from dotenv import load_dotenv
import os
# ============================================================
# CONFIGURAÇÕES
# ============================================================
load_dotenv()

USER = os.getenv("MONGO_USER")
PASS = os.getenv("MONGO_PASS")
HOST = os.getenv("MONGO_HOST")
DB = os.getenv("MONGO_DB")
API_KEY = os.getenv("API_KEY")

# URI Dinâmica e Segura
MONGO_URI = f"mongodb://{HOST}:27017/{DB}"

DB_NAME = os.getenv("MONGO_DB")
COLLECTION = "novo_monitoramento"

# Definição dos diretórios 
DATA_DIR = "data"
STATIC_DIR = "static"

# Cria as pastas se não existirem
os.makedirs(DATA_DIR, exist_ok=True)
os.makedirs(STATIC_DIR, exist_ok=True)

app = Flask(__name__, static_folder=STATIC_DIR, template_folder=STATIC_DIR)
socketio = SocketIO(app, cors_allowed_origins="*", async_mode="threading")

client = MongoClient(MONGO_URI)
db = client[DB_NAME]
colecao = db[COLLECTION]

# Horário local do Brasil
BRASIL = timezone(timedelta(hours=-3))


# ============================================================
# SISTEMA DE CLASSIFICAÇÃO (RULE-BASED)
# ============================================================

def definir_estado_predito(d):
    mq3 = d["mq3_raw"]
    temp = d["temperatura"]
    umid = d["umidade_ar"]
    fruta = d["tipoFruta"].lower()

    # ===========================
    # TOMATE → MODELO POR PERCENTUAL
    # Esta seção é usada quando tipoFruta = "tomate"
    # ===========================
    if fruta == "tomate":

        # baseline realista para o ambiente do tomate
        baseline = 930  # ambiente fresco

        aumento = ((mq3 - baseline) / baseline) * 100  # porcentagem

        if aumento < 7:
            return "sem risco"

        elif aumento < 15:
            return "madura"

        elif aumento < 25:
            return "alerta"

        else:
            return "risco de perda"

    # ===========================
    # OUTRAS FRUTAS (Ex: BANANA) → seu sistema atual
    # Esta seção é usada para qualquer outra fruta (exceto tomate)
    # ===========================
    if mq3 < 1900:
        return "sem risco"

    elif 1900 <= mq3 <= 2600:
        return "madura"

    elif 2600 < mq3 <= 3100:
        return "alerta"

    else:
        return "risco de perda"


def calcular_validade(estado, temp, umid):

    if estado == "sem risco":
        return None

    if estado == "madura":
        base = 48  # horas
        if temp > 32:
            base -= 12
        if umid > 78:
            base -= 6
        return max(base, 6)

    if estado == "alerta":
        return 12

    if estado == "risco de perda":
        return 0

    return None


# ============================================================
# FETCH E LIMPEZA
# ============================================================

def fetch_raw_df(tipo=None, limit=2000):
    query = {}
    if tipo:
        query["tipoFruta"] = tipo

    docs = list(colecao.find(query).sort("dataRegistro", -1).limit(limit))

    if not docs:
        return pd.DataFrame()

    df = pd.DataFrame(docs)

    numeric_cols = ["temperatura", "umidade_ar", "mq3_raw", "mq3_tensao"]
    for c in numeric_cols:
        df[c] = pd.to_numeric(df.get(c), errors="coerce")

    df["timestamp"] = pd.to_datetime(df["dataRegistro"], errors="coerce")

    return df


def clean_and_engineer(df):
    if df.empty:
        return df

    df = df.sort_values("timestamp")
    df = df.ffill().bfill()

    df_num = df.set_index("timestamp")[["temperatura", "umidade_ar", "mq3_raw", "mq3_tensao"]]

    agg = df_num.resample("30min").mean().dropna().reset_index()
    agg["mq3_slope"] = agg["mq3_raw"].diff().fillna(0)

    return agg


# ============================================================
# EXPORTAÇÃO CSV
# ============================================================

def exportar_csv_por_fruta(tipo):
    # Encontra registros de um tipo específico (ex: banana ou tomate)
    registros = list(colecao.find({"tipoFruta": tipo}).sort("dataRegistro", 1))

    if not registros:
        return None

    caminho = os.path.join(DATA_DIR, f"{tipo}.csv")

    with open(caminho, "w", newline="", encoding="utf-8") as csvfile:
        writer = csv.writer(csvfile)

        # Cabeçalho do CSV
        writer.writerow([
            "timestamp", "tipoFruta", "lote",
            "temperatura", "umidade_ar", "mq3_raw",
            "mq3_tensao", "estado_previsto",
            "estado_real", "validade"
        ])

        # Escreve os dados
        for r in registros:
            writer.writerow([
                r.get("dataRegistro"),
                r.get("tipoFruta"),
                r.get("lote"),
                r.get("temperatura"),
                r.get("umidade_ar"),
                r.get("mq3_raw"),
                r.get("mq3_tensao"),
                r.get("estado_previsto"),
                r.get("estado_real"),
                r.get("validade")
            ])

    return caminho


# ============================================================
# REGRESSÃO LINEAR (ANÁLISE DE TENDÊNCIA)
# ============================================================

def regressao_linear(df):
    if df.empty:
        return None

    df = df.sort_values("timestamp")
    df["t"] = range(len(df))  # índice temporal

    X = df[["t"]]
    y = df["mq3_raw"]

    model = LinearRegression()
    model.fit(X, y)

    return {
        "coeficiente": float(model.coef_[0]),
        "intercepto": float(model.intercept_),
        "tendencia": "subindo" if model.coef_[0] > 0 else "caindo"
    }


# ============================================================
# ROTAS
# ============================================================

@app.route("/api/novo_monitoramento", methods=["POST"])
def receber_dados():
    # VERIFICAÇÃO DE SEGURANÇA
    key = request.headers.get("X-API-KEY")
    if key != API_KEY:
        return jsonify({"erro": "Acesso negado"}), 403
    
    try:
        data = request.get_json()

        if not data:
            return jsonify({"erro": "JSON vazio"}), 400

        # CAMPOS OBRIGATÓRIOS
        obrig = ["tipoFruta", "temperatura", "umidade_ar", "mq3_raw", "mq3_tensao"]
        for o in obrig:
            if o not in data:
                return jsonify({"erro": f"Faltando campo: {o}"}), 400

        # CAMPOS OPCIONAIS
        data["lote"] = data.get("lote", None)
        data["estado_real"] = data.get("estado_real", None)

        # CLASSIFICAÇÃO
        estado_prev = definir_estado_predito(data)
        validade = calcular_validade(estado_prev, data["temperatura"], data["umidade_ar"])

        data["estado_previsto"] = estado_prev
        data["validade"] = validade

        # HORÁRIO DO BRASIL (datetime)
        data["dataRegistro"] = datetime.now(BRASIL)

        # SALVAR NO MONGO
        result = colecao.insert_one(data)
        
        # =======================================================
        # CORREÇÃO PARA O ERRO: ObjectId is not JSON serializable (HTTP 500)
        # -------------------------------------------------------
        # Converte o ObjectId gerado pelo MongoDB para string,
        # resolvendo o problema de serialização na resposta JSON.
        data["_id"] = str(result.inserted_id)
        # =======================================================

        # CONVERTER DATA DE DATETIME PARA STRING JSON
        data["dataRegistro"] = data["dataRegistro"].isoformat()

        payload = {
            "id": str(result.inserted_id),
            **data
        }

        socketio.emit("new_data", payload)

        return jsonify(payload)

    except Exception as e:
        # Tenta lidar com o erro 500, garantindo que o ESP32 não trave
        if "ObjectId" in str(e):
             # O dado foi salvo, mas a resposta JSON falhou. Retornamos 200 para o ESP32.
             return jsonify({"mensagem": "Dados salvos com sucesso, erro na formatação da resposta."}), 200

        return jsonify({"erro": str(e)}), 500

@app.route("/exportar/<tipo>")
def exportar_tipo(tipo):
    # Rota usada para exportar: http://192.168.0.15:8080/exportar/banana
    # Também pode ser usada para http://192.168.0.15:8080/exportar/tomate
    if tipo == "todas":
        frutas = colecao.distinct("tipoFruta")
        return jsonify({
            "arquivos": [exportar_csv_por_fruta(f) for f in frutas]
        })

    caminho = exportar_csv_por_fruta(tipo)
    if caminho:
        return jsonify({"arquivo": caminho})

    return jsonify({"erro": "Nada encontrado"}), 404


@app.route("/api/fruta/<tipo>/limpo")
def fruta_limpa(tipo):
    df = fetch_raw_df(tipo)
    if df.empty:
        return jsonify([])

    clean = clean_and_engineer(df)
    clean["timestamp"] = clean["timestamp"].astype(str)

    return clean.to_json(orient="records")


@app.route("/api/fruta/<tipo>/regressao")
def fruta_regressao(tipo):
    df = fetch_raw_df(tipo)
    if df.empty:
        return jsonify({"erro": "Sem dados"})

    r = regressao_linear(df)
    return jsonify(r)


@app.route("/dashboard")
def dash():
    return render_template("dashboard.html")


@app.route("/static/<path:path>")
def static_files(path):
    return send_from_directory("static", path)


# ============================================================
# MAIN
# ============================================================

if __name__ == "__main__":
    # O endereço 0.0.0.0 garante que o Flask aceite conexões de
    # outros dispositivos na rede local (como o ESP32 no 192.168.0.15).
    print("Servidor rodando na sua rede local (verifique seu IP): Porta 8080")
    socketio.run(app, host=os.getenv("IP"), port=8080)
    
    