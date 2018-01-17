del *.dat
conv220.exe -t themes.txt themes20.txt
conv220.exe -f text1.txt text20_1.txt
conv220.exe -f text2.txt text20_2.txt
conv220.exe -f refers.txt refers20.txt
setkeys.exe keywords.txt text20_1.txt text20k1.txt 
setkeys.exe keywords.txt text20_2.txt text20k2.txt 
setkeys.exe keywords.txt tables20.txt tables2k.txt
grad20.exe -t themes20.txt
grad20.exe text20k1.txt
grad20.exe text20k2.txt
grad20.exe refers20.txt
grad20.exe tables2k.txt
