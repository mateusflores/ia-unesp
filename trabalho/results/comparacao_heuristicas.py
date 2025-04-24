import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import pearsonr, ttest_ind
import numpy as np
import seaborn as sns

# ===== Definição dos DataFrames individuais =====
# sa_mht.dat
data_sa_mht = {
    'Algorithm': ['SA_Manhattan']*10,
    'Moves': [10682, 7586, 10271, 36235, 27549, 15364, 11310, 51161, 16534, 6782],
    'Time': [0.00182, 0.15353, 0.02944, 0.01910, 0.03189, 0.03018, 0.00238, 0.16155, 0.31208, 0.00095],
    'Result': ['V']*10
}
df_sa_mht = pd.DataFrame(data_sa_mht)

# sa_pop.dat
data_sa_pop = {
    'Algorithm': ['SA_Pieces_out_of_place']*10,
    'Moves': [16316, 9766, 707, 6054, 23703, 22276, 12993, 23634, 12208, 3762],
    'Time': [0.14042, 0.00122, 0.01255, 0.00075, 0.02789, 0.15288, 0.02650, 0.05314, 0.05151, 0.02538],
    'Result': ['V']*10
}
df_sa_pop = pd.DataFrame(data_sa_pop)

#lbs_mht.dat
data_lbs_mht = {
    'Algorithm': ['LBS_Manhattan']*10,
    'Moves': [190851, 190878, 99953, 11988, 99963, 668, 69920, 99955, 69924, 99953],
    'Time': [0.15245, 0.15225, 0.10189, 0.01901, 0.10227, 0.05129, 0.02891, 0.08539, 0.02708, 0.07068],
    'Result': ['V']*10
}
df_lbs_mht = pd.DataFrame(data_lbs_mht)

#lbs_pop.dat
data_lbs_pop = {
    'Algorithm': ['LBS_Pieces_out_of_place']*10,
    'Moves': [122348, 81162, 172519, 104270, 37740, 59330, 47980, 47974, 46888, 81692],
    'Time': [0.03403, 0.05827, 0.08678, 0.07642, 0.06614, 0.05441, 0.00737, 0.00726, 0.02257, 0.05801],
    'Result': ['V']*10
}
df_lbs_pop = pd.DataFrame(data_lbs_pop)

#hc_mht.dat
data_hc_mht = {
    'Algorithm': ['HC_Manhattan']*10,
    'Moves': [999482, 128674, 100350, 14342, 999502, 167118, 53110, 750782, 716624, 514626],
    'Time': [0.17153, 0.02203, 0.01728, 0.00246, 0.17079, 0.02851, 0.00908, 0.12842, 0.11811, 0.07247],
    'Result': ['D', 'V', 'V', 'V', 'D', 'V', 'V', 'V', 'V', 'V']
}
df_hc_mht = pd.DataFrame(data_hc_mht)

#hc_pop.dat
data_hc_pop = {
    'Algorithm': ['HC_Pieces_out_of_place']*10,
    'Moves': [606026, 999900, 93316, 550748, 542050, 807924, 369926, 209836, 13404, 596464],
    'Time': [0.09200, 0.15211, 0.01411, 0.08349, 0.08241, 0.12297, 0.05494, 0.02708, 0.00190, 0.07534],
    'Result': ['V', 'D', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V']
}
df_hc_pop = pd.DataFrame(data_hc_pop)

#bfs.dat
data_bfs = {
    'Algorithm': ['BFS']*10,
    'Moves': [20, 24, 22, 26, 18, 28, 18, 24, 20, 26],
    'Time': [11.49168, 132.72770, 35.54142, 387.07862, 3.27248, 570.11209, 1.42791, 130.39138, 8.09516, 292.40911],
    'Result': ['V']*10
}
df_bfs = pd.DataFrame(data_bfs)

# Combinando todos os algoritmos em um único DF
dfs = [
    pd.DataFrame(data_sa_mht),
    pd.DataFrame(data_sa_pop),
    pd.DataFrame(data_lbs_mht),
    pd.DataFrame(data_lbs_pop),
    pd.DataFrame(data_hc_mht),
    pd.DataFrame(data_hc_pop),
    pd.DataFrame(data_bfs)
]
df_all = pd.concat(dfs, ignore_index=True)

import seaborn as sns
import matplotlib.pyplot as plt

plt.figure(figsize=(12, 6))
sns.boxplot(data=df_all, x='Algorithm', y='Time')
plt.title('Distribuição dos Tempos de Execução')
plt.xticks(rotation=45)
plt.grid(True, axis='y', linestyle='--', alpha=0.5)
plt.show()
