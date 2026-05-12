SELECT
    estado_real,
    estado_previsto,
    ROUND(AVG(validade), 2) AS media_validade,
    COUNT(*) AS total
FROM monitoramento_frutas
WHERE validade IS NOT NULL
GROUP BY estado_real, estado_previsto
ORDER BY media_validade DESC;