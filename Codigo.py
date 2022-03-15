import pandas as pd 
import matplotlib.pyplot

arquivo_txt = pd.read_csv('DATA.txt', header= None, skiprows = 1)
ValoresTemperaturas = []
DatasEHoras = []

for valor in arquivo_txt[0]:
    dados = valor.split(" ")
    DataEHora = (str(dados[3]) + str(dados[4]))
    DatasEHoras.append(DataEHora)
    ValoresTemperatura = float(dados[6])
    ValoresTemperaturas.append(ValoresTemperatura)
    
    
matplotlib.pyplot.plot(DatasEHoras,ValoresTemperaturas)
matplotlib.pyplot.xticks(rotation=45)
matplotlib.pyplot.xlabel("Tempo (s)", size = 10)
matplotlib.pyplot.ylabel("Temperatura (°C)", size = 10)
matplotlib.pyplot.tick_params(axis='x', labelsize=8)
matplotlib.pyplot.tick_params(axis='y', labelsize=8)

matplotlib.pyplot.show()
