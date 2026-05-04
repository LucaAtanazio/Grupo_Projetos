SELECT 
    EXTRACT(HOUR FROM data_registro) as hora_do_dia,
    ROUND(AVG(temperatura), 2) as media_temperatura,
    MAX(temperatura) as pico_maximo
FROM monitoramento_frutas
GROUP BY hora_do_dia
ORDER BY hora_do_dia;