#!/bin/bash

# ===================================================================
# Script Shell
# ===================================================================

# Chemin de l'exécutable C généré par le Makefile
EXEC_PATH="codeC/cwire"

# Fonction d'affichage de l'aide
show_help() {
    echo "Qu'est-ce que vous pouvez faire :"
    echo "  $0 <fichier.csv> histo <max|src|real>"
    echo "  $0 <fichier.csv> leaks <identifiant_usine>"
    exit 1
}

# Début du chronométrage
start_time=$(date +%s%N)

# 1. Vérification des arguments
if [ "$#" -ne 3 ]; then
    echo "Erreur : Nombre d'arguments incorrect (3 attendus)."
    show_help
fi

DATA_FILE="$1"
COMMAND="$2"
ARGUMENT="$3"

# Vérification du fichier d'entrée
if [ ! -f "$DATA_FILE" ]; then
    echo "Erreur : Le fichier de données '$DATA_FILE' introuvable."
    exit 1
fi

# 2. Vérification et Compilation du programme C
# On vérifie si l'exécutable existe dans codeC/
if [ ! -f "$EXEC_PATH" ] && [ ! -f "${EXEC_PATH}.exe" ]; then
    echo "L'exécutable est absent. Compilation en cours..."
    make
    if [ $? -ne 0 ]; then
        echo "Erreur : La compilation a échoué."
        exit 1
    fi
fi

# 3. Traitement
echo "Lancement du traitement C..."

# Appel du programme C
./"$EXEC_PATH" "$DATA_FILE" "$COMMAND" "$ARGUMENT"
RET=$?

# Vérification du succès du programme C
if [ $RET -ne 0 ]; then
    echo "Le programme C a renvoyé une erreur (Code $RET)."
    exit $RET
fi

OUTPUT_DAT="vol_${ARGUMENT}.dat"
    

    if [ -f "$OUTPUT_DAT" ]; then
        echo "Fichier de données trouvé : $OUTPUT_DAT"

        gnuplot -e "
            set terminal png size 1200,800;
            set output 'graph_${ARGUMENT}.png'; 
            set style data histograms;
            set style fill solid;
            set title 'Histogramme des usines (${ARGUMENT})';
            set xlabel 'Usines';
            set ylabel 'Volume (M.m3)';
            plot '$OUTPUT_DAT' using 2:xtic(1) title '${ARGUMENT}';
        "
        
        echo "Graphique généré avec succès : graph_${ARGUMENT}.png"
    else
        echo "Erreur : Le fichier '$OUTPUT_DAT' n'a pas été généré par le programme C."
        exit 1
    fi

elif [ "$COMMAND" = "leaks" ]; then
    ./"$EXEC_PATH" "$DATA_FILE" "$COMMAND" "$ARGUMENT"
    RET=$?

    # Vérification du succès du programme C
    if [ $RET -ne 0 ]; then
        echo "Le programme C a renvoyé une erreur (Code $RET)."
    exit $RET
    fi
    echo "Calcul des fuites terminé. Vérifiez leaks.csv."

fi

# 5. Affichage du temps d'exécution
end_time=$(date +%s%N)
duration=$(( (end_time - start_time) / 1000000 )) # Conversion en ms
echo "Durée totale du traitement : ${duration} ms"
