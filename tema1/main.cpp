#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

ifstream f("date.in");

/**
  * Simboluri
  * ----------------
  * NOT        : !
  * AND        : &
  * OR         : |
  * IMPLICA    : -
  * ECHIVALENT : =
  * PROPOZITIE : A, B, C, ...
  *
  * !! Propozitiile nu pot contine doua sau mai multe caractere ( Ex: A2, AB, AB4, etc ) !!
  *
  * Ex: ((A&B)|(!C))
  */


char Formula[100];

/**
  * Aici este stocata formula initiala, cu mentiunea ca se face abstractie de propozitii atomice,
  * insemnand faptul ca nu se mai face diferenta intre ele.
  *
  * Ex: Formula A&B devine p&p
  */
char Formula_abstracta[100];


/*************************************************************************************
 *                     VERIFICARE FORMULA PROPOZITIONALA                             *
 *************************************************************************************/


/**
  * Aceasta functie genereaza formula abstracta pe baza originalului, inlocuind
  * propozitiile atomice cu p, intrucat valorea lor de adevar nu are relevanta
  * in verificarea sintaxei.
  */
void Abstractizare_Formula ( )
{
    for ( unsigned i = 0 ; i < strlen(Formula) ; i++ )
        if ( ! strchr( "&-=|!()" , Formula[i] ) )
            Formula_abstracta[i] = 'p' ;
        else Formula_abstracta[i] = Formula[i] ;
}


/**
  * Aceasta functie numara cate paranteze au fost deschise si cate au fost inchise.
  * Daca cele doua numere sunt egale, atunci returneaza numarul de paranteze
  * deschise, ce coincide cu numarul de perechi de paranteze, altfel returneaza 0
  * intrucat formula nu este valida.
  *
  * Acesta este primul pas in validarea formulei.
  */
unsigned Valideaza_Paranteze ( )
{
    unsigned Paranteze_deschise = 0 ;
    unsigned Paranteze_inchise = 0 ;

    for ( unsigned  i = 0 ; i < strlen(Formula_abstracta) ; i++ )
    {
        if ( Formula_abstracta[i] == '(' )
            Paranteze_deschise++ ;
        if ( Formula_abstracta[i] == ')' )
            Paranteze_inchise++ ;
    }

    if ( Paranteze_deschise == Paranteze_inchise )
        return Paranteze_deschise ;
    else {
        cout << "EROARE!\n";
        return 0;
    }
}


/**
  * Aceasta functie numara cati conectori se gasesc in formula si returneaza
  * numarul acestora.
  */
unsigned Numara_Conectori ( )
{
    unsigned Numar_conectori = 0 ;

    for ( unsigned i = 0 ; i < strlen(Formula_abstracta) ; i++ )
        if ( strchr( "&-=!|" , Formula_abstracta[i] ) )
            Numar_conectori++ ;

    return Numar_conectori ;
}


/**
  * Aceasta functie reprezinta ultimul pas din validarea formulei, pe care o
  * verifica caracter cu caracter.
  */
void Verificare_Formula ( unsigned & True )
{

    cout << "    2.1. Se verifica daca primul si ultimul caracter formeaza o pereche de paranteze...";
    if ( Formula_abstracta[0] == '(' && Formula_abstracta[ strlen(Formula_abstracta) - 1 ] == ')' )
    {
        cout << "OK!\n";
        /**
          * Contorizam cate paranteze au ramas deschise.
          *
          * Daca, in orice moment, valoarea scade sub 0, atunci formula e gresita.
          * Daca, la final, valoarea este mai mare ca 1, atunci formula e gresita.
          */
        int Contor_paranteze = 0 ;
        unsigned Gasit_conector = 0 ;

        for ( unsigned i = 0 ; i < strlen(Formula_abstracta) - 1 && True == 1 ; i++ )
        {
            cout << "    2." << i+2 << ". Se verifica caracterul " << i+1 << " din formula...";

            if ( Formula_abstracta[i] == '(' )
            {
                Contor_paranteze++;
                cout << " am deschis o paranteza... [ " << Contor_paranteze << " ] ...";

                if ( ! strchr( "p!(" , Formula_abstracta[i+1] ) ) {
                    cout << " EROARE!\n";
                    True = 0 ;
                }
                
                cout << "\n";
            }

            if ( Formula_abstracta[i] == 'p' ) {
                if ( ! strchr( "&|-=)" , Formula_abstracta[i+1] ) ) {
                    cout << " EROARE!\n";
                    True = 0 ;
                }
            }

            if ( strchr( "&|!-=" , Formula_abstracta[i] ) )
            {
                Gasit_conector++ ;
                cout << " am gasit un conector... [ " << Gasit_conector << " ] ...";

                if ( ! strchr( "p(" , Formula_abstracta[i+1] ) ) {
                    True = 0 ;
                    cout << " EROARE!\n";
                }
            }

            if ( Formula_abstracta[i] == ')' )
            {
                Contor_paranteze-- ;
                cout << " am inchis o pereche de paranteze... [ " << Contor_paranteze << " ] ...";

                if ( Gasit_conector == 0) {
                    cout << " EROARE! Nu exista conectori in aceasta pereche de paranteze.\n";
                    True = 0 ;
                } else {
                    Gasit_conector-- ;
                    cout << " Am eliminat un conector... [ " << Gasit_conector << " ] ...";
                }

                if ( ! strchr( ")&-=|" , Formula_abstracta[i+1] ) ) {
                    True = 0 ;
                    cout << " EROARE!\n";
                }
            }

            if ( Contor_paranteze < 0 ) {
                True = 0 ;
                cout << " !!! EROARE GENERALA !!! Se incearca inchiderea unei perechi de paranteze inexistente !\n";
            }
         
            if( True == 1 ) {
                cout << " OK!\n";
            }
        }

        if ( Contor_paranteze > 1 ) {
            True = 0 ;
            cout << " !!! EROARE GENERALA !!! Nu s-au inchis toate parantezele !\n";
        }
    }
    else {
        True = 0 ;
        cout << " EROARE!\n";
    }
}



/*************************************************************************************
 *                                  MAIN                                             *
 *************************************************************************************/

int main()
{
    f >> Formula ;

    cout << "0. Se inlocuiesc propozitiile atomice cu p...  ";
 
    Abstractizare_Formula( ) ;

    cout  << " OK! [ " << Formula_abstracta << " ]\n";
    cout << "0.1. Se valideaza perechile de paranteze...";
 
    unsigned Perechi_paranteze = Valideaza_Paranteze() ;
 
    if(Perechi_paranteze > 0)
        cout << " OK!\n";
    cout << "0.2. Se numara conectorii din formula...";
 
    unsigned Numar_de_conectori = Numara_Conectori() ;

    cout << " OK!\n";
 
    /**
      * Variabila True retine valorea 1 daca sirul de simboluri este o formula propozitonala,
      * iar in caz contrar retine valoarea 0.
      */
    unsigned True ;

    /**
      * Daca numarul de perechi de paranteze este diferit de numarul de conectori, din start
      * sirul nu poate reprezenta o formula propozitionala.
      */
    cout << "1.0. Se verifica daca perechile de paranteze si conectorii coincid la numar...";
 
    if ( Perechi_paranteze == Numar_de_conectori ) {
        True = 1;
        cout << " OK!\n";
    } else {
        True = 0;
        cout << " EROARE!\n"
    }

    /**
      * Daca strlen(Formula_abstracta) == 1 inseamna ca avem o propozitie atomica si nu este
      * necesara verificarea.
      */
    if ( True == 1 && strlen(Formula_abstracta) > 1 ) {
        cout << "2.0. Se verifica formula caracter cu caracter...\n";
        Verificare_Formula(True) ;
    }

    if ( True == 1 )
        cout << Formula << " este o formula propozitionala " << endl ;
    else cout << Formula << " nu este o formula propozitionala " << endl ;

    return 0;
}
