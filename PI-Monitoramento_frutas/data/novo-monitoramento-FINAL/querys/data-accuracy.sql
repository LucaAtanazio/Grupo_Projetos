SELECT 
    tipo_fruta, 
    COUNT(*) as total_registros,
    COUNT(*) FILTER (WHERE estado_real = estado_previsto) as acertos_ia,
    ROUND((COUNT(*) FILTER (WHERE estado_real = estado_previsto)::numeric / COUNT(*)) * 100, 2) as porcentagem_acerto
FROM monitoramento_frutas
GROUP BY tipo_fruta;