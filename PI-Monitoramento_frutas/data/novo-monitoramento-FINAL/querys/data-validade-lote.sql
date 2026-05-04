SELECT 
    lote, 
    ROUND(AVG(validade), 1) as dias_restantes_media,
    COUNT(*) as amostras
FROM monitoramento_frutas
WHERE lote IS NOT NULL
GROUP BY lote
ORDER BY dias_restantes_media ASC;