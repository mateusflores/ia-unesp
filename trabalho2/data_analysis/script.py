import pandas as pd
import re
import io
import matplotlib.pyplot as plt
import seaborn as sns

file_content = """
File: data_10.txt
BFS: 0.00148722 0.000870968
DIJ: 0.00245074 0.000611008
HBFS: 0.000649347 0.000123368
HDIJ: 0.0011791 0.000314232

File: data_20.txt
BFS: 0.00343156 0.00222211
DIJ: 0.0083034 0.00439556
HBFS: 0.000823624 0.000194285
HDIJ: 0.00167648 0.000921872

File: data_30.txt
BFS: 0.00747988 0.00716963
DIJ: 0.0196595 0.0108583
HBFS: 0.000900319 0.000213562
HDIJ: 0.00218034 0.000533748

File: data_40.txt
BFS: 0.0121618 0.00966352
DIJ: 0.0369126 0.0193523
HBFS: 0.0010627 0.000662911
HDIJ: 0.00300232 0.00713158

File: data_50.txt
BFS: 0.0141938 0.00842806
DIJ: 0.0552311 0.0387171
HBFS: 0.00108131 0.000311755
HDIJ: 0.00295257 0.00130546
"""

def parse_results_to_dataframe(content):
    data = []
    current_file = None
    algo_map = {
        'BFS': 'Busca em Largura (BFS)',
        'DIJ': 'Dijkstra',
        'HBFS': 'BFS Hierárquico',
        'HDIJ': 'Dijkstra Hierárquico'
    }
    for line in content.strip().split('\n'):
        line = line.strip()
        if not line: continue
        file_match = re.match(r"File: (data_\d+\.txt)", line)
        if file_match:
            current_file = file_match.group(1)
            continue
        result_match = re.match(r"(\w+): ([\d.]+) ([\d.]+)", line)
        if result_match and current_file:
            algo_short_name = result_match.group(1)
            mean_time = float(result_match.group(2))
            std_dev = float(result_match.group(3))
            num_edges = int(re.search(r'\d+', current_file).group(0))
            data.append({
                'Nº de Arestas': num_edges,
                'Algoritmo': algo_map.get(algo_short_name, algo_short_name),
                'Tempo Médio (ms)': mean_time,
                'Desvio Padrão (ms)': std_dev
            })
    return pd.DataFrame(data)

results_df = parse_results_to_dataframe(file_content)

# Análise de Escalabilidade (Gráfico de Linhas)

sns.set_theme(style="whitegrid")

plt.figure(figsize=(10, 6))

lineplot = sns.lineplot(
    data=results_df,
    x='Nº de Arestas',
    y='Tempo Médio (ms)',
    hue='Algoritmo',
    style='Algoritmo',  
    markers=True,     
    dashes=False,
    linewidth=2.5
)

plt.title('Performance vs. Tamanho do Grafo (Escalabilidade)', fontsize=16)
plt.xlabel('Número de Arestas no Grafo', fontsize=12)
plt.ylabel('Tempo Médio de Execução (ms)', fontsize=12)
plt.legend(title='Algoritmo')
plt.grid(True)

plt.show()

# Performance e Variabilidade (Gráfico de Barras)

data_50_arestas = results_df[results_df['Nº de Arestas'] == 50].copy()

data_50_arestas.sort_values('Tempo Médio (ms)', ascending=False, inplace=True)

plt.figure(figsize=(10, 6))

barplot = plt.bar(
    x=data_50_arestas['Algoritmo'],
    height=data_50_arestas['Tempo Médio (ms)'],
    yerr=data_50_arestas['Desvio Padrão (ms)'],
    capsize=5, 
    color=sns.color_palette("viridis", n_colors=len(data_50_arestas))
)

plt.title('Performance no Grafo de 50 Arestas', fontsize=16)
plt.ylabel('Tempo Médio de Execução (ms)', fontsize=12)
plt.xticks(rotation=15, ha="right") 

plt.show()

# Ganho Hierárquico (Tabela de Speedup)

pivot_df = results_df.pivot_table(
    index='Nº de Arestas', 
    columns='Algoritmo', 
    values='Tempo Médio (ms)'
)

pivot_df['Speedup BFS (x)'] = pivot_df['Busca em Largura (BFS)'] / pivot_df['BFS Hierárquico']
pivot_df['Speedup Dijkstra (x)'] = pivot_df['Dijkstra'] / pivot_df['Dijkstra Hierárquico']

speedup_table = pivot_df[['Speedup BFS (x)', 'Speedup Dijkstra (x)']].copy()
speedup_table = speedup_table.round(1)

print("--- Tabela de Aceleração (Speedup) da Busca Hierárquica ---")
print(speedup_table)