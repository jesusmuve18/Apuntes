import pandas as pd
import seaborn as sns
import os

def main():
    fnames:list[str] = [f for f in os.listdir(".") if ".csv" in f]
    for fname in fnames:
        df = pd.read_csv(fname)
        df['alg'] = df['alg'].str.upper()
        p = sns.catplot(data=df, x="alg", y="fitness", aspect=1, kind="box")
        p.set(xlabel="Algoritmo", ylabel="Fitness")
        foutput = fname.replace(".csv", ".png")
        p.savefig(foutput)
        
if __name__ == '__main__':
    main()

