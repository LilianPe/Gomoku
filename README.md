# Gomoku

## Launch
```bash
make run
```

## Regles:

Doit pouvoir jouer soit :  
- Contre lui meme, doit toujours gagner en commencant  
- Contre un joueur avec une feature de suggestion de mouvement  

- Pour gagner: Alligner 5 pions sans qu'un puisse etre manger  / manger 5 paires adverse
- Pour manger une paire: 0112 -> 2002
- 

## Algo : Min-Max
- Doit avoir au moins 10 de profondeur  
- Definir une fonction heuristique : Renvoie a quelle point une position est gagnante  
- Ajouter une option de debug pour voir le raisonnement de l'IA  

## Interface graphique
- SFML ?
- Ajouter un timer affichant le temps de reflexion de l'IA  

## Bonus : 5 bonus pas de requirement
- Menu de selection de mode (VS IA / Joueur)  
- Selection de regle (chiant de faire differentes regle)  
- ?  
- ?  
- ?  
