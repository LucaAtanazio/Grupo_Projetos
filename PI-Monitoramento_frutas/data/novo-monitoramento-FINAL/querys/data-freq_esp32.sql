SELECT 
    data_registro,
    LAG(data_registro) OVER (ORDER BY data_registro) as registro_anterior,
    data_registro - LAG(data_registro) OVER (ORDER BY data_registro) as intervalo
FROM monitoramento_frutas
WHERE tipo_fruta = 'banana'
LIMIT 20;