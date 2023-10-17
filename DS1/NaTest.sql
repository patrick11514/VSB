SELECT p.*
FROM poslanec p
JOIN organy org ON p.id_obdobi = org.id_organ
WHERE YEAR(org.od_organ) = 2013