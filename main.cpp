#include <iostream>
#include <time.h>
#include <fstream>
#include <iomanip>
using namespace std;

class Dane{
  public:
    int id;
    int stream[20];
    int sumtime;
};

void przerzuc (int first, int n, Dane *pierwszy, Dane *drugi)
{
    for (int i=0; i<n; i++){
        drugi[i+first] = pierwszy[i];
    }
}

void SumTimeFoTask (int n, int m, Dane dane[])
{
    int sum = 0;
    for (int i=0; i<n; i++){
        for (int j=0; j<m; j++){
            sum += dane[i].stream[j];
        }
        dane[i].sumtime = sum;
        sum = 0;
    }
}

void sortTime (int n, Dane *dane)
{
    for (int i=0; i<n; i++){
        for (int j=n-1; j>i; j--){
            if (dane[j].sumtime > dane[j-1].sumtime){
                swap(dane[j], dane[j-1]);
            }
        }
    }
}

int timeCmax (int n, int m, Dane dane[])
{
    int Cmax[m];
    Cmax[0] = dane[0].stream[0];
    for (int i=1; i<m; i++){
        Cmax[i] = Cmax[i-1] + dane[0].stream[i];
    }
    for (int i=1; i<n; i++){
        Cmax[0] += dane[i].stream[0];
        for (int j=1; j<m; j++){
            Cmax[j] = max(Cmax[j-1], Cmax[j]) + dane[i].stream[j];
        }
    }
    return Cmax[m-1];
}

void sortQueue (int n, int m, Dane *dane)
{
    Dane obliczenia[n];
    Dane wynik[n];
    wynik[0] = dane[0];
    int mintime, timeC;
    for (int i=1; i<n; i++){
        obliczenia[0] = dane[i];
        przerzuc(1, i, wynik, obliczenia);
        przerzuc(0, i+1, obliczenia, wynik);
        mintime = timeCmax(i+1, m, obliczenia);
        for (int j=0; j<i; j++){
            swap (obliczenia[j], obliczenia[j+1]);
            timeC = timeCmax(i+1, m, obliczenia);
            if (timeC < mintime){
                mintime = timeC;
                przerzuc(0, i+1, obliczenia, wynik);
            }
        }
    }
    przerzuc(0, n, wynik, dane);
}

void timeCmaxLeftToRight (int last, int m, Dane dane[], int **Cmax)
{
    Cmax[0][0] = dane[0].stream[0];
    for (int i=1; i<m; i++){
        Cmax[0][i] = Cmax[0][i-1] + dane[0].stream[i];
    }
    for (int i=1; i<last; i++){
        Cmax[i][0] = Cmax[i-1][0] + dane[i].stream[0];
        for (int j=1; j<m; j++){
            Cmax[i][j] = max(Cmax[i][j-1], Cmax[i-1][j]) + dane[i].stream[j];
        }
    }
}

void timeCmaxRightToLeft (int last, int m, Dane dane[], int **Cmax)
{
    --last;
    Cmax[last][m-1] = dane[last].stream[m-1];
    for (int i=m-2; i>-1; i--){
        Cmax[last][i] = Cmax[last][i+1] + dane[last].stream[i];
    }
    for (int i=last-1; i>-1; i--){
        Cmax[i][m-1] = Cmax[i+1][m-1] + dane[i].stream[m-1];
        for (int j=m-2; j>-1; j--){
            Cmax[i][j] = max(Cmax[i][j+1], Cmax[i+1][j]) + dane[i].stream[j];
        }
    }
}

int qTimeCmax (int ind, int m, int ile, int dane[], int *Cmltr[], int *Cmrtl[])
{
    int Cmax[m], Cm;
    if (ind == 0){
        Cmax[m-1] = Cmrtl[0][m-1] + dane[m-1];
        for (int i=m-2; i>-1; i--){
            Cmax[i] = max(Cmax[i+1], Cmrtl[0][i]) + dane[i];
        }
        return Cmax[0];
    }
    if (ind == ile){
        Cmax[0] = Cmltr[ind-1][0] + dane[0];
        for (int i=1; i<m; i++){
            Cmax[i] = max(Cmax[i-1], Cmltr[ind-1][i]) + dane[i];
        }
        return Cmax[m-1];
    }

    Cmax[0] = Cmltr[ind-1][0] + dane[0];
    Cm = Cmax[0] + Cmrtl[ind][0];
    for (int i=1; i<m; i++){
        Cmax[i] = max(Cmax[i-1], Cmltr[ind-1][i]) + dane[i];
        if (Cmax[i]+Cmrtl[ind][i] > Cm){
            Cm = Cmax[i] + Cmrtl[ind][i];
        }
    }
    return Cm;
}

void qsortQueue (int n, int m, Dane *dane, int **CmLeftToRight, int **CmRightToLeft)
{
    Dane obliczenia[n];
    Dane wynik[n];
    wynik[0] = dane[0];
    int mintime, timeC, ind=0;
    for (int i=1; i<n; i++){
        timeCmaxLeftToRight(i, m, wynik, CmLeftToRight);
        timeCmaxRightToLeft(i, m, wynik, CmRightToLeft);
        ind=0;
        obliczenia[0] = dane[i];
        przerzuc(1, i, wynik, obliczenia);
        przerzuc(0, i+1, obliczenia, wynik);
        mintime = qTimeCmax (ind, m, i, dane[i].stream, CmLeftToRight, CmRightToLeft);
        for (int j=0; j<i; j++){
            swap (obliczenia[j], obliczenia[j+1]);
            timeC = qTimeCmax (j+1, m, i, dane[i].stream, CmLeftToRight, CmRightToLeft);
            if (timeC < mintime){
                mintime = timeC;
                ind = j + 1;
                przerzuc(0, i+1, obliczenia, wynik);
            }
        }
    }
    przerzuc(0, n, wynik, dane);
}

void sorting (int n,  int m, Dane *dane)
{
    int timemin = timeCmax(n, m, dane), timeC;
    for (int i=0; i<n; i++){
        for (int j=i+1; j<n; j++){
            swap(dane[i], dane[j]);
            timeC = timeCmax(n, m, dane);
            if (timeC < timemin){
                timemin = timeC;
                i=0; j=1;
            } else {
                swap(dane[i], dane[j]);
            }
        }
    }
}

int main()
{
    int odp[121];
    odp[0] = 32;
    odp[1] = 1286;
    odp[2] = 1365;
    odp[3] = 1159;
    odp[4] = 1325;
    odp[5] = 1305;
    odp[6] = 1228;
    odp[7] = 1278;
    odp[8] = 1223;
    odp[9] = 1291;
    odp[10] = 1151;
    odp[11] = 1680;
    odp[12] = 1729;
    odp[13] = 1557;
    odp[14] = 1439;
    odp[15] = 1502;
    odp[16] = 1453;
    odp[17] = 1562;
    odp[18] = 1609;
    odp[19] = 1647;
    odp[20] = 1653;
    odp[21] = 2410;
    odp[22] = 2150;
    odp[23] = 2411;
    odp[24] = 2262;
    odp[25] = 2397;
    odp[26] = 2349;
    odp[27] = 2362;
    odp[28] = 2249;
    odp[29] = 2320;
    odp[30] = 2277;
    odp[31] = 2733;
    odp[32] = 2843;
    odp[33] = 2640;
    odp[34] = 2782;
    odp[35] = 2868;
    odp[36] = 2850;
    odp[37] = 2758;
    odp[38] = 2721;
    odp[39] = 2576;
    odp[40] = 2790;
    odp[41] = 3135;
    odp[42] = 3032;
    odp[43] = 2986;
    odp[44] = 3198;
    odp[45] = 3160;
    odp[46] = 3178;
    odp[47] = 3277;
    odp[48] = 3123;
    odp[49] = 3002;
    odp[50] = 3257;
    odp[51] = 4082;
    odp[52] = 3921;
    odp[53] = 3927;
    odp[54] = 3969;
    odp[55] = 3835;
    odp[56] = 3914;
    odp[57] = 3952;
    odp[58] = 3938;
    odp[59] = 3952;
    odp[60] = 4079;
    odp[61] = 5519;
    odp[62] = 5348;
    odp[63] = 5219;
    odp[64] = 5023;
    odp[65] = 5266;
    odp[66] = 5139;
    odp[67] = 5259;
    odp[68] = 5120;
    odp[69] = 5489;
    odp[70] = 5341;
    odp[71] = 5846;
    odp[72] = 5453;
    odp[73] = 5824;
    odp[74] = 5929;
    odp[75] = 5679;
    odp[76] = 5375;
    odp[77] = 5704;
    odp[78] = 5760;
    odp[79] = 6032;
    odp[80] = 5918;
    odp[81] = 6541;
    odp[82] = 6523;
    odp[83] = 6639;
    odp[84] = 6557;
    odp[85] = 6695;
    odp[86] = 6664;
    odp[87] = 6632;
    odp[88] = 6739;
    odp[89] = 6677;
    odp[90] = 6677;
    odp[91] = 10942;
    odp[92] = 10716;
    odp[93] = 11025;
    odp[94] = 11057;
    odp[95] = 10645;
    odp[96] = 10458;
    odp[97] = 10989;
    odp[98] = 10829;
    odp[99] = 10574;
    odp[100] = 10807;
    odp[101] = 11594;
    odp[102] = 11675;
    odp[103] = 11852;
    odp[104] = 11803;
    odp[105] = 11685;
    odp[106] = 11629;
    odp[107] = 11833;
    odp[108] = 11913;
    odp[109] = 11673;
    odp[110] = 11869;
    odp[111] = 26670;
    odp[112] = 27232;
    odp[113] = 26848;
    odp[114] = 27055;
    odp[115] = 26727;
    odp[116] = 26992;
    odp[117] = 26797;
    odp[118] = 27138;
    odp[119] = 26631;
    odp[120] = 26984;

    string plik, nazwa = "dane", roz = ".txt";
    char p = '0', d = '0', t; //pirwsza, druga, trzecia liczby
    int n, m;
    bool tf;
    double totaltime = 0;
    for (int j=0; j<121; j++){
        t = (j%10)+48;
        d = ((j/10)%10)+48;
        p = ((j/100)%10)+48;
        plik = nazwa + p + d + t + roz;
        ifstream dane_input(plik);

        dane_input >> n >> m;
        Dane *dane = new Dane[n];
        int **CmLeftToRight = new int*[n];
        int **CmRightToLeft = new int*[n];
        for (int i=0; i<n; i++){
            CmLeftToRight[i] = new int[m];
            CmRightToLeft[i] = new int[m];
            for (int j=0; j<m; j++){
                CmLeftToRight[i][j] = -1;
                CmRightToLeft[i][j] = -1;
            }
        }
        for (int i=0; i<n; i++){
            dane[i].id = i+1;
            for (int j=0; j<m; j++){
                dane_input >> dane[i].stream[j];
            }
        }

        SumTimeFoTask(n, m, dane);
        sortTime(n, dane);
        clock_t start = clock();
            qsortQueue(n, m, dane, CmLeftToRight, CmRightToLeft);
            //sortQueue(n, m, dane);
            //sorting(n, m, dane);
        clock_t end = clock();
        double elapsed = double(end - start)/CLOCKS_PER_SEC;

        if (odp[j]==timeCmax(n, m, dane)){
            tf = true;
        } else {
            tf = false;
        }

        cout << plik << "  " << setw(5) << timeCmax(n, m, dane) << "  " << setw(6)
             << fixed << setprecision(3) << elapsed << "   " << tf << endl;
             totaltime += elapsed;
    }
    cout << "Total Time: " << totaltime << endl;

    return 0;
}
