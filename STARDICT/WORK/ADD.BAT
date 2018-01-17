del *.dat
del *.tmp
del *.bak
grad themes.txt -t
copy text.txt + tables.txt tmp.txt
grad tmp.txt
del tmp.txt
grad refers.txt