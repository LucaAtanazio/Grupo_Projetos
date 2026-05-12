SELECT
    lote,
    COUNT(*) AS total,
    COUNT(*) FILTER (
        WHERE estado_real = estado_previsto
    ) AS acertos,
    ROUND(
        (
            COUNT(*) FILTER (
                WHERE estado_real = estado_previsto
            )::numeric / COUNT(*)
        ) * 100,
        2
    ) AS accuracy
FROM monitoramento_frutas
GROUP BY lote;