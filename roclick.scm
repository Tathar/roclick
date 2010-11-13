(verbose)

;(button-grab No_du_bouton_a_intercepter)
;"button-grab" intercepte compeltement le signal du bouton,qui devient donc inutilisable pour autre chose
;ne pas utiliser "button-grab" avec le bouton 1, sous risque de petit soucis ;-)
(button-grab 8)
(button-grab 9)



;(key-use "Non_de_la_touche_a_intercepter")
(key-use "F5")
(key-use "F6")

;(key-use "Non_de_la_touche_a_utiliser")
(key-mod "space")
(key-mod "greater")

;"key-use" intercepte completement le signal de la touche,comme "button-grab"
;alor que "key-mod" vous permet d'utiliser la touche normalement, pratique pour la touche "espace" par exemple

;roclick-KR execute la lambda expression au moment ou l'on relache la touche ou le bouton selectionne
(roclick-KR "F5" (lambda () (display "F5") (newline) (key-press "Return") (string "merci, gracias, thanks") (key-press "Return") ))

;la fonction (key-press "Non_de_la_touche") simule l'appuie sur la touche nomme
;la fonction (string "une chaine de caracteres") ecrit la chaine de caracteres

(roclick-KR "F6" (lambda () (display "F6") (newline) (key-press "Return") (string "de rien, de nada, don't mention it") (key-press "Return") ))

;roclick-KP execute la lambda expression au moment ou l'on appui sur la touche ou le bouton selectionne
(roclick-KP "space" (lambda () (display "Barre 4") (newline) (key-press "F4") ))
(roclick-KR "space" (lambda () (display "Barre 3") (newline) (key-press "F3") ))

(roclick-KP "greater" (lambda () (display "Barre 2") (newline) (key-press "F2") ))
(roclick-KR "greater" (lambda () (display "Barre 3") (newline) (key-press "F3") ))

;la touche viruel "all" signifie "n'importe quel key-mod cumule,meme aucun"
;Ok c'est pa tres claire mes tu n'a qu'a faire des essais
(roclick "all 9" (lambda () (display "KP_1") (newline) (key-press "KP_1") ))

(roclick "all 8" (lambda () (display "KP_2") (newline) (key-press "KP_2") ))

(roclick "all 9 8" (lambda () (display "KP_3") (newline) (key-press "KP_3") ))
(roclick "all 8 9" (lambda () (display "KP_3") (newline) (key-press "KP_3") ))

(roclick "all 9 3" (lambda () (display "KP_4") (newline) (key-press "KP_4") ))

(roclick "all 8 3" (lambda () (display "KP_5") (newline) (key-press "KP_5") ))

(roclick "all 9 1" (lambda () (display "KP_6") (newline) (key-press "KP_6") ))

(roclick "all 8 1" (lambda () (display "KP_7") (newline) (key-press "KP_7") ))

(roclick "all 9 4" (lambda () (display "KP_8") (newline) (key-press "KP_8") ))

(roclick "all 9 5" (lambda () (display "KP_9") (newline) (key-press "KP_9") ))

(roclick "all 9 8 3" (lambda () (display "KP_0") (newline) (key-press "KP_0") ))
(roclick "all 8 9 3" (lambda () (display "KP_0") (newline) (key-press "KP_0") ))

