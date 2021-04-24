import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns
from scipy import fft
import numpy as np
from datetime import datetime
from sklearn import preprocessing
from tqdm import tqdm

min_max_scaler = preprocessing.MinMaxScaler()

data_raw = pd.read_csv('../example-data/DATA.CSV',
                       sep=';',
                       header=None,
                       names=['date', 'hour', 'weekday', 'RH', 'P', 'T', 'CO', 'CO2', 'PM'],
                       parse_dates=[['date', 'hour']],
                       usecols=range(9),
                       dayfirst=True)

data_raw.rename(columns={'date_hour':'date'},
                inplace=True)

data = data_raw

data = data.set_index('date')

data.head()

# data_scaled = min_max_scaler.fit_transform(data)
# data_scaled = pd.DataFrame(data_scaled)