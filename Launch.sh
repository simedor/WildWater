#!/bin/bash
# [Launch.sh]

EXEC="codeC/cwire"
MAKEFILE="Makefile"

show_help() {
    echo "Usage du programme : $0 <fichier.csv> <histo|leaks> <args>"
    echo "Utilisation pour 'histo' : src | max | real"
    echo "Utilisation pour 'leaks' : <ID_usine>"
    exit 0
}

if [ "$1" == "-h" ] || [ "$#" -ne 3 ]; then
    show_help
fi

DATA_FILE="$1"
COMMAND="$2"
ARGUMENT="$3"

# Compilation automatique si nécessaire
if [ ! -f "$EXEC" ]; then
    echo "Compilation..."
    make
    if [ $? -ne 0 ]; then exit 1; fi
fi

# Nettoyage des anciens graphiques
rm -f *.png

# Exécution du programme C
echo "Exécution C..."
start=$(date +%s%N)
./$EXEC "$DATA_FILE" "$COMMAND" "$ARGUMENT"
if [ $? -ne 0 ]; then exit 1; fi
end=$(date +%s%N)
duration=$(( (end - start) / 1000000 ))

# --- LOGIQUE MIN / MAX pour HISTO ---
if [ "$COMMAND" = "histo" ]; then
    if [ "$ARGUMENT" == "src" ]; then CSV_OUT="vol_source.csv"; COL=2; fi
    if [ "$ARGUMENT" == "max" ]; then CSV_OUT="vol_max.csv"; COL=2; fi
    if [ "$ARGUMENT" == "real" ]; then CSV_OUT="vol_real.csv"; COL=2; fi
    
    # 1. Trier pour les 10 plus GRANDS (ordre décroissant 'r')
    # On saute la 1ère ligne (header) avec tail, on trie, on prend les 10 premiers
    echo "Génération du Top 10..."
    cat "$CSV_OUT" | head -n 1 > "header.temp"
    tail -n +2 "$CSV_OUT" | sort -t';' -k${COL}nr | head -n 10 > "data_max.temp"
    cat "header.temp" "data_max.temp" > "temp_top10.csv"

    # 2. Trier pour les 50 plus PETITS (ordre croissant)
    echo "Génération du Top 50 min..."
    tail -n +2 "$CSV_OUT" | sort -t';' -k${COL}n | head -n 50 > "data_min.temp"
    cat "header.temp" "data_min.temp" > "temp_min50.csv"

    # 3. Gnuplot : Fonction pour générer un graph
    generate_graph() {
        INPUT=$1
        OUTPUT=$2
        TITLE=$3
        gnuplot -e "
            set terminal png size 1000,600 enhanced font 'Arial,10';
            set output '$OUTPUT';
            set datafile separator ';';
            set title '$TITLE';
            set style data histograms;
            set style fill solid 1.0 border -1;
            set boxwidth 0.7;
            set xtics rotate by -45 scale 0;
            set ylabel 'Volume (M.m3)';
            plot '$INPUT' using ${COL}:xtic(1) title 'Volume' linecolor rgb '#4caf50';
        "
    }

    generate_graph "temp_top10.csv" "${ARGUMENT}_max.png" "Top 10 : $ARGUMENT"
    generate_graph "temp_min50.csv" "${ARGUMENT}_min.png" "Top 50 Min : $ARGUMENT"

    # Nettoyage temp
    rm *.temp
fi

echo "Terminé en ${duration} ms."