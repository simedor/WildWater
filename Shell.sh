#!/bin/bash

# Arguments :
# $1 : fichier CSV de données
# $2 : commande (histo ou leaks)
# $3 : sous-commande (max, src, real) ou identifiant usine

# Début du chronométrage (en nanosecondes pour la précision)
start_time=$(date +%s%N)

# Nom de l'exécutable C
EXEC="final"

# Fonction d'affichage de l'aide
show_help() {
    echo "Usage :"
    echo "  $0 <fichier.csv> histo <max|src|real>"
    echo "  $0 <fichier.csv> leaks <identifiant_usine>"
    exit 1
}
if [ "$#" -ne 3 ]; then
    echo "Erreur : Nombre d'arguments incorrect."
    show_help
fi

DATA_FILE="$1"
COMMAND="$2"
ARGUMENT="$3"

if [ ! -f "$DATA_FILE" ]; then
    echo "Erreur : Le fichier de données '$DATA_FILE' n'existe pas."
    exit 1
fi

# 2. Vérification et Compilation du programme C
if [ ! -f "$EXEC" ]; then
    echo "L'exécutable '$EXEC' est absent. Compilation en cours..."
    make
    if [ $? -ne 0 ]; then
        echo "Erreur : La compilation a échoué."
        exit 1
    fi
fi

# 3. Traitement selon la commande
if [ "$COMMAND" = "histo" ]; then
    # Vérification du sous-argument
    if [[ "$ARGUMENT" != "max" && "$ARGUMENT" != "src" && "$ARGUMENT" != "real" ]]; then
        echo "Erreur : Argument '$ARGUMENT' invalide pour histo. Utilisez max, src ou real." [cite: 179, 180]
        exit 1
    fi

    echo "Génération de l'histogramme ($ARGUMENT)..."
    
    # Appel du programme C [cite: 139]
    ./$EXEC "$DATA_FILE" "$COMMAND" "$ARGUMENT"
    RET=$?

    if [ $RET -ne 0 ]; then
        echo "Erreur : Le programme C a rencontré un problème (Code $RET)." [cite: 198]
        exit $RET
    fi

    # Génération du graphique avec Gnuplot (Histogramme) 
    # Le programme C est censé avoir généré un fichier .dat (ex: vol_max.dat)
    # On suppose ici que le fichier généré s'appelle "output.dat" pour l'exemple.
    # Adaptez le nom selon ce que votre C produit.
    
    OUTPUT_DAT="output.dat" 
    
    if [ -f "$OUTPUT_DAT" ]; then
        gnuplot -e "
            set terminal png size 1200,800;
            set output '${ARGUMENT}.png';
            set style data histograms;
            set style fill solid;
            set title 'Histogramme des usines ($ARGUMENT)';
            set xlabel 'Usines';
            set ylabel 'Volume (M.m3)';
            plot '$OUTPUT_DAT' using 2:xtic(1) title '$ARGUMENT';
        "
        echo "Graphique généré : ${ARGUMENT}.png"
    else
        echo "Erreur : Le fichier de données pour le graphique n'a pas été trouvé."
    fi

elif [ "$COMMAND" = "leaks" ]; then
    # Appel du programme C pour les fuites
    # L'identifiant peut contenir des espaces, donc on utilise des guillemets "$ARGUMENT"
    ./$EXEC "$DATA_FILE" "$COMMAND" "$ARGUMENT"
    RET=$?

    if [ $RET -ne 0 ]; then
        echo "Erreur : Le programme C a échoué ou l'usine n'existe pas (Code $RET)."
        # Note : Si l'usine n'existe pas, le sujet dit d'écrire -1 dans le fichier, 
        exit $RET
    fi
    
    echo "Calcul des fuites terminé. Vérifiez le fichier de sortie."

else
    echo "Erreur : Commande '$COMMAND' inconnue."
    show_help
fi

# 4. Affichage du temps d'exécution [cite: 199]
end_time=$(date +%s%N)
duration=$(( (end_time - start_time) / 1000000 )) # Conversion en millisecondes

echo "Durée totale du traitement : ${duration} ms"

