import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns
import numpy as np
from sklearn import preprocessing
from sklearn.decomposition import PCA
import winsound
from mpl_toolkits.mplot3d import Axes3D
from plotnine import *

def beep():
    import time
    bpm = 300
    quiver = int(60000/bpm) # beep milliseconds

    d4 = 294
    g4 = 392
    a4 = 440
    bb4 = 466
    c5 = 523
    d5 = 587

    winsound.Beep(d4, quiver)
    winsound.Beep(g4, quiver)
    winsound.Beep(bb4, quiver)
    winsound.Beep(d5, quiver)
    time.sleep(quiver / 1000)
    winsound.Beep(d5, quiver)
    winsound.Beep(c5, quiver)
    winsound.Beep(bb4, quiver)
    winsound.Beep(a4, quiver)
    winsound.Beep(bb4, 3*quiver)
    pass

min_max_scaler = preprocessing.MinMaxScaler()
sns.set_theme(style="whitegrid")

beep()
#%%
data_raw = pd.read_csv('./example-data/DATA.CSV',
                       sep=';',
                       header=None,
                       names=['date', 'hour', 'weekday', 'RH', 'P', 'T', 'CO', 'CO2', 'PM'],
                       parse_dates=[['date', 'hour']],
                       usecols=range(9),
                       dayfirst=True)
data_raw.rename(inplace=True,
                columns={'date_hour': 'date'})
beep()
#%%
data = data_raw.copy()
data = data.set_index('date')
data = data.resample('1min').mean()
data['date'] = data.index
data = data[data.index > '2021-01-01']

beep()
#%%
dplot = data.copy()
sns.set_style("dark")
g = sns.lineplot(data=dplot,
                 x=dplot.index,
                 y='T',
                 ci=None)
plt.setp(g.collections, sizes=[0])
plt.show()

beep()
#%%
dplot = data.copy()
dplot['part'] = np.where(dplot.index < '2021-01-29', 'A', 'B')
plt.plot(dplot['T'], label = 'Temperature')
plt.xlabel('date')
plt.ylabel('T')
plt.legend()
plt.show()

beep()
#%%
dplot = data.copy()
dplot['part'] = np.where(dplot.index < '2021-01-29', 'A', 'B')
print(ggplot(dplot, aes(x='date', y='T'))
     + geom_line(aes(color='part')))
beep()
#%%
dplot = data.copy()
sns.scatterplot(x="T", y="PM",
                hue="CO", size="CO2",
                sizes=(1, 8), linewidth=0,
                data=dplot)
plt.show()

beep()
#%%
dplot = data.copy()
dplot = dplot[dplot.PM > 50]
interest_vars = ['T','PM']
sns.lmplot(x=interest_vars[0], y=interest_vars[1], size=10,
           palette="viridis",
           data=dplot)
sns.histplot(x=interest_vars[0], y=interest_vars[1],
             bins=100, pthresh=.1, cmap="viridis",
             data=dplot)
plt.show()

beep()
#%%
dplot = data.copy()
dplot = dplot[dplot.PM > 100]
# Compute a correlation matrix and convert to long-form
corr_mat = dplot.corr().stack().reset_index(name="correlation")

# Draw each cell as a scatter point with varying size and color
g = sns.relplot(
    data=corr_mat,
    x="level_0", y="level_1", hue="correlation", size="correlation",
    palette="vlag", hue_norm=(-1, 1), edgecolor=".7",
    height=10, sizes=(50, 250), size_norm=(-.2, .8),
)

# Tweak the figure to finalize
g.set(xlabel="", ylabel="", aspect="equal")
g.despine(left=True, bottom=True)
g.ax.margins(.02)
for label in g.ax.get_xticklabels():
    label.set_rotation(90)
for artist in g.legend.legendHandles:
    artist.set_edgecolor(".7")

plt.show()

beep()
#%%
dplot = data.copy().drop(columns=['weekday','date'])
dplot = dplot[dplot.PM > 100]
# dplot['part'] = np.where(dplot.index < '2021-01-29', 'A', 'B')
# dplot = dplot[dplot.part == 'A'].drop(columns='part')

fig = plt.figure(1, figsize=(8, 6))
ax = Axes3D(fig, elev=-150, azim=110)
X_reduced = PCA(n_components=3).fit_transform(dplot[(dplot != 'part')])


ax.scatter(X_reduced[:, 0], X_reduced[:, 1], X_reduced[:, 2], c=dplot.PM,
           cmap=plt.cm.Set1, edgecolor='k', s=40)
ax.set_title("First three PCA directions")
ax.set_xlabel("1st eigenvector")
ax.w_xaxis.set_ticklabels([])
ax.set_ylabel("2nd eigenvector")
ax.w_yaxis.set_ticklabels([])
ax.set_zlabel("3rd eigenvector")
ax.w_zaxis.set_ticklabels([])

plt.show()