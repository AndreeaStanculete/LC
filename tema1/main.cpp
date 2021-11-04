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
    else
    {
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
        cout << "OK!" << endl ;
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

                if ( ! strchr( "p!(" , Formula_abstracta[i+1] ) )
                {
                    cout << " EROARE!" << endl ;
                    True = 0 ;
                }

                cout << endl ;
            }

            if ( Formula_abstracta[i] == 'p' )
            {
                if ( ! strchr( "&|-=)" , Formula_abstracta[i+1] ) )
                {
                    cout << " EROARE!" << endl ;
                    True = 0 ;
                }
            }

            if ( strchr( "&|!-=" , Formula_abstracta[i] ) )
            {
                Gasit_conector++ ;
                cout << " am gasit un conector... [ " << Gasit_conector << " ] ...";

                if ( ! strchr( "p(" , Formula_abstracta[i+1] ) )
                {
                    True = 0 ;
                    cout << " EROARE!" << endl ;
                }
            }

            if ( Formula_abstracta[i] == ')' )
            {
                Contor_paranteze-- ;
                cout << " am inchis o pereche de paranteze... [ " << Contor_paranteze << " ] ...";

                if ( Gasit_conector == 0)
                {
                    cout << " EROARE! Nu exista conectori in aceasta pereche de paranteze." << endl ;
                    True = 0 ;
                }
                else
                {
                    Gasit_conector-- ;
                    cout << " Am eliminat un conector... [ " << Gasit_conector << " ] ...";
                }

                if ( ! strchr( ")&-=|" , Formula_abstracta[i+1] ) )
                {
                    True = 0 ;
                    cout << " EROARE!" << endl ;
                }
            }

            if ( Contor_paranteze < 0 )
            {
                True = 0 ;
                cout << " !!! EROARE GENERALA !!! Se incearca inchiderea unei perechi de paranteze inexistente !" << endl;
            }

            if( True == 1 )
            {
                cout << " OK!" << endl ;
            }
        }

        if ( Contor_paranteze > 1 )
        {
            True = 0 ;
            cout << " !!! EROARE GENERALA !!! Nu s-au inchis toate parantezele !" << endl ;
        }
    }
    else
    {
        True = 0 ;
        cout << " EROARE!" << endl ;
    }
}


/*************************************************************************************
 *                             FORMARE ARBORE                                        *
 *************************************************************************************/


 struct nod
{
	struct nod* Copil_stanga ;
	struct nod* Copil_dreapta ;
	struct nod* Parinte ;
	char Simbol ;
	int Valoare ;
} n;

struct arbore
{
	nod* Radacina ;
} Arbore ;

 /**
   * Aceasta functie formeaza arborele abstract pe baza unei formule stricte.
   */
void Formare_Arbore()
{
    nod* nodCurent = Arbore.Radacina;

    for ( unsigned i = 0 ; i < strlen(Formula) ; i++ )
    {
        if ( Formula[i] == '(' )
        {
            nod* Element = new nod() ;
            Element->Copil_dreapta = NULL ;
            Element->Copil_stanga = NULL ;
            Element->Parinte = NULL ;
            Element->Simbol = '#' ;
            Element->Valoare = 2 ;

            if ( nodCurent == NULL )
            {
                Arbore.Radacina = Element ;
                nodCurent = Element ;
            }
            else
            {
                if ( nodCurent->Copil_stanga == NULL )
                    nodCurent->Copil_stanga = Element ;
                else
                    nodCurent->Copil_dreapta = Element ;

                Element->Parinte = nodCurent ;
                nodCurent = Element ;
            }
        }
        else if ( ! strchr( "&-=|!)" , Formula[i] ) )
        {
            nod* Element = new nod() ;
            Element->Copil_dreapta = NULL ;
            Element->Copil_stanga = NULL ;
            Element->Parinte = NULL ;
            Element->Simbol = Formula[i] ;
            f >> Element->Valoare ;

            if ( nodCurent->Copil_stanga == NULL )
                nodCurent->Copil_stanga = Element ;
            else
                nodCurent->Copil_dreapta = Element ;

            Element->Parinte = nodCurent ;
        }
        else if ( Formula[i] != ')' )
            nodCurent->Simbol = Formula[i] ;
        else if ( Formula[i] == ')' )
            nodCurent = nodCurent->Parinte ;
    }
}


/**
  * Aceasta este o functie recursiva care afiseaza pentru fiecare nod Parinte copiii sai.
  */
void Afisare_Nod( nod* ptr , int Nivel )
{
    if ( ptr != NULL && strchr( "!&-=|" , ptr->Simbol ) )
    {
        if ( ptr->Simbol == '!' )
        {
            cout << ptr->Simbol << " ,situat pe nivelul " << Nivel << " are ca si copil: " ;
            if ( ptr->Copil_stanga != NULL )
                cout << ptr->Copil_stanga->Simbol << endl ;
            if ( ptr->Copil_dreapta != NULL )
                cout << ptr->Copil_dreapta->Simbol << endl ;
        }
        else
        {
            cout << ptr->Simbol << " ,situat pe nivelul " << Nivel << " are ca si copii: " << endl ;

            if ( ptr->Copil_stanga != NULL )
                cout << "    - la stanga: " << ptr->Copil_stanga->Simbol << endl ;
            if ( ptr->Copil_dreapta != NULL )
                cout << "    - la dreapta: " << ptr->Copil_dreapta->Simbol << endl ;
        }

        cout << endl ;
        Afisare_Nod( ptr->Copil_stanga , Nivel + 1 ) ;
        Afisare_Nod( ptr->Copil_dreapta ,Nivel + 1 ) ;
    }
}


/*************************************************************************************
 *            TRANSFORMARE DIN SINTAXA RELAXATA IN SINTAXA STRICTA                   *
 *************************************************************************************/

/**
  * Variabila Operatori retine pe prima coloana pozitia fiecarui operator logic din
  * Formula, iar pe a doua coloana importanta fiecaruia, calculata

  * Variabila Nr_op retine numarul de operatori din formula ( numarul de linii
  * din matricea Operatori )
  */
int Operatori[100][2] ;
int Nr_op ;


/**
  * Tabloul unidimensional retine pozitia fiecarei paranteze deschise si inchise, prin
  * atribuirea unei valori ( 1 daca e paranteza deschisa si 2 daca e paranteza inchisa ).
  * Pentru toate celelalte caractere din Formula ramane valoarea 0.
  */
int Paranteze[100] ;
void Retine_Pozitia_Parantezelor()
{
    for ( int i = 0 ; i < strlen(Formula) ; i++ )
    {
        if ( Formula[i] == '(' )
            Paranteze[i] = 1 ;
        if ( Formula[i] == ')' )
            Paranteze[i] = 2 ;
    }
}


/**
  * Folosim functia Sortare pentru a sorta matricea Operatori descrescator dupa importanta
  * operatorilor, astfel incat operatorul cu cea mai mare importanta se afla pe prima linie,
  * iar operatorul cu cea mai mica impoertanta pe ultima linie.
  */
void Sortare()
{
    for ( int i = 0 ; i < Nr_op - 1 ; i++ )
        for ( int j = i + 1 ; j < Nr_op ; j++ )
            if ( Operatori[i][1] < Operatori[j][1] )
            {
                int aux = Operatori[i][0] ;
                Operatori[i][0] = Operatori[j][0] ;
                Operatori[j][0] = aux ;
                aux = Operatori[i][1] ;
                Operatori[i][1] = Operatori[j][1] ;
                Operatori[j][1] = aux ;

            }
}


/**
  * Aceasta functie calculeaza importanta fiecarui operator dupa formula:
  * importanta = Numar de paranteze deschise * 50 + Importanta de start

  * importanta de start este atribuita fiecarui operator in functie de cat de tare leaga.
  * NOT !         : 20
  * AND &         : 15
  * OR  |         : 10
  * IMPLICA -     :  5
  * ECHIVALENT =  :  0
  */
void Calcul_Importanta()
{
    unsigned Bonus_paranteza=50;
    int Paranteza_deschisa=0;

    for ( unsigned i = 0 ; i < strlen(Formula) ; i++ )
    {
        if ( Formula[i] == '(' )
            Paranteza_deschisa++ ;
        else if (Formula[i] == ')' )
            Paranteza_deschisa-- ;
        else if ( strchr( "!&|-=" , Formula[i] ) )
        {
            Operatori[Nr_op][0] = i ;
            Operatori[Nr_op][1] = Paranteza_deschisa * Bonus_paranteza ;

            if (Formula[i] == '!' )
                Operatori[Nr_op][1]+= 20 ;
            else if ( Formula[i] == '&' )
                Operatori[Nr_op][1]+= 15 ;
            else if (Formula[i] == '|' )
                Operatori[Nr_op][1]+= 10 ;
            else if (Formula[i] == '-' )
                Operatori[Nr_op][1]+= 5 ;

            Nr_op++;
        }
    }
}

/**
  * Aceasta functie modifica formula din sintaxa relaxata in sintaxa stricta,
  * prin adaugarea de paranteze la stanga si la dreapta operatorilor, daca este cazul,
  * in ordinea importantei lor.
  */
char aux[100];
void Transformare_Formula_Relaxata_in_Formula_Stricta()
{
    Calcul_Importanta();
    Sortare();

    Retine_Pozitia_Parantezelor();

    int introdusParanteza ;
    for (int i = 0 ; i < Nr_op; i++)
    {
        introdusParanteza = 0;
        int j = Operatori[i][0];

        if ( Formula[j] == '!' )
        {
            if ( j == 0 )
            {
                strcpy( aux , Formula + j ) ;
                Formula[j]= '(' ;
                strcpy( Formula + j + 1 , aux ) ;
                introdusParanteza = 1;
            }
            else if ( Paranteze[j-1] != 1 )
            {
                strcpy( aux , Formula + j );
                Formula[j] = '(' ;
                strcpy( Formula + j + 1 , aux ) ;
                introdusParanteza = 1;
            }

            for ( int k = 0 ; k < Nr_op && introdusParanteza == 1 ; k++ )
                if( Operatori[k][0] >= j)
                    Operatori[k][0]++;

            if ( introdusParanteza == 1 )
            {
                for ( int k = j + 1 ; k < strlen(Formula) ; k++ )
                {
                    Paranteze[k] = Paranteze[k-1];
                }
                Paranteze[j] = 0;
            }
        }
        else if ( strchr( "&|-=" , Formula[j] ) )
        {
            while ( strchr( "&|-=(" , Formula[j] ) )
                j--;

            if ( Formula[j] == ')' )
            {
                int Contor = 1;

                while( Contor > 0)
                {
                    j--;

                    if( Formula[j] == '(' )
                        Contor-- ;
                    else if ( Formula[j] == ')')
                        Contor++ ;
                }

                if ( j == 0 )
                {
                    strcpy( aux , Formula + j ) ;
                    Formula[j] = '(' ;
                    strcpy( Formula + j + 1 , aux ) ;
                    introdusParanteza = 1;
                }
                else if ( Paranteze[j-1] != 1 )
                {
                    strcpy( aux , Formula + j );
                    Formula[j] = '(' ;
                    strcpy( Formula + j + 1 , aux ) ;
                    introdusParanteza = 1 ;
                }

                for ( int k = 0 ; k < Nr_op && introdusParanteza == 1 ; k++ )
                    if( Operatori[k][0] >= j )
                        Operatori[k][0]++ ;

            }
            else
            {
                if ( j == 0 )
                {
                    strcpy( aux , Formula + j ) ;
                    Formula[j] = '(' ;
                    strcpy( Formula + j + 1 , aux ) ;
                    introdusParanteza = 1;
                }
                else if ( Paranteze[j-1] !=1 )
                {
                    strcpy( aux , Formula + j ) ;
                    Formula[j] = '(' ;
                    strcpy( Formula + j + 1 , aux ) ;
                    introdusParanteza = 1;
                }

                for ( int k = 0; k < Nr_op && introdusParanteza == 1 ; k++ )
                    if( Operatori[k][0] >= j )
                        Operatori[k][0]++ ;
            }
        }

        j = Operatori[i][0];
        introdusParanteza = 0;

        while ( strchr( "!&|-=)" , Formula[j] ) )
            j++ ;

        if ( Formula[j] == '(' )
        {
            int Contor = 1;

            while ( Contor > 0)
            {
                j++;

                if ( Formula[j] == '(' )
                    Contor++;
                else if ( Formula[j] == ')' )
                    Contor--;
            }

            if ( j == strlen(Formula) - 1 )
            {
                Formula[j+1] = ')' ;
                Formula[j+2] = '\0';
            }
            else if ( Paranteze[j+1] != 2 )
            {
                strcpy( aux , Formula + j + 1 ) ;
                Formula[j+1] = ')' ;
                strcpy( Formula + j + 2 , aux ) ;
                introdusParanteza = 1;
            }
        }
        else
        {
            if ( j == strlen(Formula) - 1 )
            {
                Formula[j+1] = ')' ;
                Formula[j+2] = '\0' ;
            }
            else if ( Paranteze[j+1] != 2 )
            {
                strcpy( aux , Formula + j + 1 ) ;
                Formula[j+1] = ')' ;
                strcpy( Formula + j + 2 , aux ) ;
                introdusParanteza = 1;
            }
        }

        for ( int k = 0; k < Nr_op && introdusParanteza == 1 ; k++ )
            if( Operatori[k][0] >= j + 1 )
                Operatori[k][0]++;

        if ( introdusParanteza == 1 )
        {
            for ( int k = j + 2 ; k < strlen(Formula) ; k++ )
                Paranteze[k] = Paranteze[k-1] ;
            Paranteze[j] = 0 ;
        }
    }
}


/*************************************************************************************
 *                            VERIFICARE ADEVAR                                      *
 *************************************************************************************/

 void Afisare_Valori_Adevar( nod* ptr )
 {
     if ( ptr != NULL )
     {
         if ( ! strchr( "!&|-=" , ptr->Simbol ) )
         {
            cout << "Propozitia " << ptr->Simbol << " are valoarea de adevar: " ;
            if ( ptr->Valoare == 0 )
                cout << "False" << endl;
            else cout << "True" << endl;
         }

         Afisare_Valori_Adevar( ptr->Copil_stanga ) ;
         Afisare_Valori_Adevar( ptr->Copil_dreapta ) ;
     }
 }


unsigned Verificare_Adevar ( nod* ptr )
{
    while ( ptr != NULL )
    {
        if ( ptr->Simbol == '!' )
        {
            return !Verificare_Adevar( ptr->Copil_stanga ) ;
        }
        else if ( strchr( "&-=|" , ptr->Simbol ) )
        {
            if ( ptr->Simbol == '&' )
                return Verificare_Adevar(ptr->Copil_dreapta) && Verificare_Adevar(ptr->Copil_stanga) ;
            else if ( ptr->Simbol == '|' )
                return Verificare_Adevar(ptr->Copil_dreapta) || Verificare_Adevar(ptr->Copil_stanga) ;
            else if ( ptr->Simbol == '-' )
            {
                if ( Verificare_Adevar(ptr->Copil_stanga) == 0 || Verificare_Adevar(ptr->Copil_dreapta) == Verificare_Adevar(ptr->Copil_stanga) )
                    return 1 ;
                return 0 ;
            }
            else if ( ptr->Simbol == '=' )
            {
                if ( Verificare_Adevar(ptr->Copil_dreapta) == Verificare_Adevar(ptr->Copil_stanga) )
                    return 1 ;
                return 0 ;
            }
        }
        else
        {
            return ptr->Valoare;
        }
    }

}



/*************************************************************************************
 *                                  MAIN                                             *
 *************************************************************************************/

int main()
{
    f >> Formula ;

    cout << "0. Se face transformarea din sintaxa relaxata in sintaxa stricta... ";

    Transformare_Formula_Relaxata_in_Formula_Stricta() ;

    cout << "1. Se inlocuiesc propozitiile atomice cu p...  ";

    Abstractizare_Formula( ) ;

    cout  << " OK! [ " << Formula_abstracta << " ]" << endl ;
    cout << "1.1. Se valideaza perechile de paranteze...";

    unsigned Perechi_paranteze = Valideaza_Paranteze() ;

    if(Perechi_paranteze > 0)
        cout << " OK!" << endl ;

    cout << "1.2. Se numara conectorii din formula...";

    unsigned Numar_de_conectori = Numara_Conectori() ;

    cout << " OK!" << endl ;

    /**
      * Variabila True retine valorea 1 daca sirul de simboluri este o formula propozitonala,
      * iar in caz contrar retine valoarea 0.
      */
    unsigned True ;

    /**
      * Daca numarul de perechi de paranteze este diferit de numarul de conectori, din start
      * sirul nu poate reprezenta o formula propozitionala.
      */
    cout << "2.0. Se verifica daca perechile de paranteze si conectorii coincid la numar...";

    if ( Perechi_paranteze == Numar_de_conectori )
    {
        True = 1;
        cout << " OK!" << endl ;
    }
    else
    {
        True = 0;
        cout << " EROARE!" << endl ;
    }

    /**
      * Daca strlen(Formula_abstracta) == 1 inseamna ca avem o propozitie atomica si nu este
      * necesara verificarea.
      */
    if ( True == 1 && strlen(Formula_abstracta) > 1 )
    {
        cout << "3.0. Se verifica formula caracter cu caracter..." << endl ;
        Verificare_Formula(True) ;
        cout << endl ;
    }

    if ( True == 1 )
    {
        cout << Formula << " este o formula propozitionala " << endl ;

        cout << endl << "----------------------------------------------------------------------" << endl << endl ;

        cout << "4.0. Se construieste arborele abstract..." << endl ;
        Formare_Arbore();
        nod* ptr=Arbore.Radacina;
        Afisare_Nod(ptr,0);

        cout << endl << "----------------------------------------------------------------------" << endl << endl ;

        Afisare_Valori_Adevar(ptr) ;
        cout << "5.0. Se calculeaza valoarea de adevar..." << endl ;
        cout << " Valoarea de adevar a formulei este: " ;
        if (Verificare_Adevar(ptr) == 1 )
            cout << "True" ;
        else cout << "False" ;

    }
    else cout << Formula << " nu este o formula propozitionala " << endl ;

    return 0;
}
