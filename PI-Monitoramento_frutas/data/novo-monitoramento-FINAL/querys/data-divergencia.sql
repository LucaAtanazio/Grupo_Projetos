SELECT
    estado_real,
    estado_previsto,
    COUNT(*) AS total
FROM monitoramento_frutas
WHERE estado_real != estado_previsto
GROUP BY estado_real, estado_previsto
ORDER BY total DESC;