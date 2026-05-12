SELECT 
    tipo_fruta, 
    ROUND(AVG(mq3_raw), 2) as media_etileno,
    COUNT(*) as total_amostras
FROM monitoramento_frutas
GROUP BY tipo_fruta
ORDER BY media_etileno DESC;