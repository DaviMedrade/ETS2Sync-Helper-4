# ETS2&ATS Sync Helper

## Support d'ETS2: version 1.39 et tous les DLCs.
## Support d'ATS: version 1.39 et tous les DLCs. (en pre release)
* [English version](README.md)

[LICENSE](LICENSE.md)

* [Téléchargement](#téléchargement)
* [Installation](#installation-up)
* [Synchroniser](#synchroniser)
* [Fontionnalités](#fontionnalités)
  * [Supprimer les missions](#supprimer-les-missions)
  * [Remise à 0 de l'économie](#remise-à-0-de-l'économie)
  * [Liste de mission](#liste-de-mission)
* [Erreurs communes](#erreurs-communes)
* [Bugs](#bugs)

Ce programme a été entièrement réécrit en C++ par David Santos, je m'occupe de maintenir le programme a jour en supportant les dernières mises à jour des jeux et les DLCs.

Ce programme syncronise votre liste de travails sur Euro Truck Simulator 2 et Americain Truck Simulator pour faciliter l'organisationb de convoi en multijoueur.

A l'heure actuelle, le programme ne fonctionne qu'en anglais. D'autres langues peuvent être ajoutées par la suite.

## Téléchargement
https://github.com/Inklay/ETS2-ATS-Sync-Helper/releases

## Installation
Lancez ETS2Sync Helper et selectionnez votre jeu (ETS2 ou ATS). Regardez l'onglet “Game Settings”. S'il y a un message vert disant “Settings OK”, tout est bon. Si le message est orange ou rouge vous devez suivre ces étapes:
1. Fermez le jeu. Ca ne marchera pas si le jeu reste ouvert.
2. Dans ETS2Sync Helper, dans l'onglet “Game Settings”, dans “Save Format”, clickez sur “Set to ‘Text’”. Le message rouge ou orange devrait devenir vert et dire “Settings OK”. ET c'est bon ! Votre jeu est prêt pour être syncronisé, vous pouvez aller à [Synchroniser](#how-to-sync).


## Synchroniser
1. Allez à l'écran d'accueil (si vous conduisez, appuyez sur <kbd>Esc</kbd>).
2. Clickez sur “Sauver & charger” → “Enregistrer la partie”.
3. Entrez un nom pour la sauvegarde ou sélectionnez en une déjà existante et écrasez là.
4. Clickez sur “Enregistrer”.
5. Faites `Alt`+`Tab` pour réduir le jeu.
6. Ouvrez ETS2Sync Helper s'il ne l'était pas déjà. Selectionnez le bon jeu dans l'option “Game” de l'onglet “Game Settings”.
7. Sélectionnez le bon profile et la bonne sauvegarde dans “Select ETS2/ATS Profile” et “Select ETS2/ATS Save”.
8. Sélectionnez les DLCs que vous et vos amis possédez (ne sélectionnez un DLC que si tout le monde l'a).
9. Clickez sur “Sync Jobs”.
10. Lorsque le message “Sync Complete” apparait, clickez sur “Close”.
11. Retournez dans le jeu.
12. Clickez sur “Sauver & charger” → “Charger une partie”.
13. Sélectionnez la sauvegarde que vous venez de syncroniser (celle que vous avez créée/remplacée à l'étape 4).
14. Clickez sur Charger.
15. Allez dans les offres de traction ou les offres de fret. Si toutes les missions ont la même date d'expiration (“l'offre expire dans”), et que cette date est d'environ 400 ou 500 heures, alors la synchronisation a bien marché.

## Fontionnalités
### Supprimer les missions
Cette fonctionnalité vous donne la possibilité de supprimer toutes vos missions. Si vous avez terminé de jouer avec les listes de missions synchronisées et que vous voulez générer de nouvelles missions pour jouer en solo, vous avez juste à suivre les mêmes étapes que pour synchroniser, mais vous dever clicker sur “Clear Jobs” au lieu de “Sync Jobs”. Lorsque vous chargerez la sauvegarde, le marché du travail sera vide. Appelez un dépanneur (<kbd>F7</kbd>) ou utilisez l'option de voyage rapide pour générer de nouvelles missions.

### Remise à 0 de l'économie
Cette fonctionnalité foncitonne comme "Supprimer les missions", mais elle est plus rapide car lorsque vous chargerez la sauvegarde vous aurez un message disant “Changements dans le jeu détectés”, qui va directement générer de nouvelles missions (vous n'avez donc pas besoin de vous déplacez ou d'appeler une dépaneuse pour avoir de nouvelles missions). Cependant cette methode vous téléportera toujours à votre garage principal.

### Liste de mission
Lors de la synchronisation, le programme télécharge une liste de mission et l'injecte dans votre sauvegarde. Il y a 8 listes pour ATS et ETS2 sur le serveur. Une de ces 8 listes sera utilisée si vous laissez l'option “Job List” sur “Automatic”. Chaque jours cette liste change (Si la liste du jour est la 7, le lendemain ça sera la 8).

Les listes de missions ne changent pas, sauf si le jeu ou les DLCs le nécessitent (Une mise à jour du jeu ou d'un DLC ou un nouveau DLC). Si vous ne voulez pas qu' votre liste de mission change d'un jour à l'autre, vous avez juste à toujours selectionner la même dans l'option “Job List”.

## Erreurs communes

### Les sauvegardes en format binaires ne sont pas supportées, veuillez créer vos sauvegarde en format text
Si vous sélectionnez une ancienne sauvegarde ou que votre jeu est mal configuré.  
* Si ETS2Sync Helper n'affiche pas un message vert disant “Settings OK” dans l'onglet “Game Settings”, vous devez faire les étapes décrire dans la section[Installation](#setting-up).
* Si le message “Settings OK” étair déjà affiché, vérifiez que vous avez sélectionné le bon profile et la bonne sauvegarde.
* SI ça ne marche toujours pas, fermez ETS2Sync Helper et le jeu, ensuite rouvrez ETS2Sync Helper et regardez si le message vert dit toujours “Settings OK”. Si ce n'est pas le cas lisez les instructions ci-dessus. Si c'est le cas, ouvrez le jeu et créez une nouvelle sauvegarde avant de synchroniser.

### Could not download the job list. InternetOpenURL error
### Could not download the job list. HTTP 500 Internal Server Error
### Could not download the job list. HTTP 401 Unauthorized
### Could not download the job list. HTTP 403 Forbidden
### Could not download the job list. HTTP 404 Not Found
Il y a très certainement un problème avec le serveur ou votre connection internet, si votre connection fonctionne correctement, attendez quelques minutes et réessayer.

## Bugs
Si vous recontrer une erreur ou un bug, merci de le reporter ici:

https://github.com/Inklay/ETS2-ATS-Sync-Helper/issues
