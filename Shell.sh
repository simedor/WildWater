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

if [ "$COMMAND" = "histo" ]; then
    if [ "$ARGUMENT" = "src" ]; then FILE="vol_source.csv"; fi
    if [ "$ARGUMENT" = "max" ]; then FILE="vol_max.csv"; fi
    if [ "$ARGUMENT" = "real" ]; then FILE="vol_real.csv"; fi

    if [ -f "$FILE" ]; then
        echo "Génération du graphique pour $FILE..."
        cat <<EOF > plot_script.gp
set terminal png size 1200,800 enhanced font 'Arial,10'
set output '${ARGUMENT}.png'
set datafile separator ';'
set title 'Histogramme : ${ARGUMENT}'
set style data histograms
set style fill solid 1.0 border -1
plot '${FILE}' using 2:xtic(1) title '${ARGUMENT}'
EOF
        
        gnuplot plot_script.gp
        rm plot_script.gp
        echo "Graphique généré : ${ARGUMENT}.png"
 else
        echo "Avertissement : Le fichier de données pour le graphique ($FILE) n'a pas été trouvé."
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
