#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "vrpsd.hpp"
#include "csv.hpp"
#include "ga.hpp"
#include "sa.hpp"

using namespace std;

float jarak[MAX_ARR][MAX_ARR];
float demand[MAX_ARR][MAX_ARR];
float f0[MAX_ARR];
float f0_best;

float fitness[MAX_ARR];
float fitness_relatif[MAX_ARR];
float fitness_kumulatif[MAX_ARR];
float fitness_total;

int rute[MAX_ARR][MAX_ARR];
int best_rute[MAX_ARR];
int point_of_best;

#include "helper.hpp"

int main()
{
    float pc, pm, psa, tmin, tnow, alpha;

    int rute_default[MAX_ARR];
    int jml_rute, i, k, l;
    int quantity = 0, population, iteration;

    bool hibrid = 1;

    /*
     * INPUT data manual
     */
    cout << "+---------------------------+" << endl;
    cout << "|          INPUT VAR        |" << endl;
    cout << "+---------------------------+" << endl;
    cout << "Hybird:";
    cin >> hibrid;
    cout << "Jumlah Rute:";
    cin >> jml_rute;
    cout << "Quantity: ";
    cin >> quantity;
    cout << "Populasi: ";
    cin >> population;
    cout << "Banyak Iterasi: ";
    cin >> iteration;
    cout << "Nilai Pc: ";
    cin >> pc;
    cout << "Nilai Pm: ";
    cin >> pm;
    cout << "Nilai Psa: ";
    cin >> psa;
    cout << "Nilai Tmin: ";
    cin >> tmin;
    cout << "Nilai Tnow: ";
    cin >> tnow;
    cout << "Nilai Alpha: ";
    cin >> alpha;
    cout << endl;

    cout << "+---------------------------+" << endl;
    cout << "|          PARAMETER        |" << endl;
    cout << "+---------------------------+" << endl;
    cout << "Hybird:" <<  hibrid << endl;
    cout << "Jumlah Rute:" << jml_rute << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Populasi: " << population << endl;
    cout << "Banyak Iterasi: " << iteration << endl;
    cout << "Nilai Pc: " << pc << endl;
    cout << "Nilai Pm: " << pm << endl;
    cout << "Nilai Psa: " << psa << endl;
    cout << "Nilai Tmin: " << tmin << endl;
    cout << "Nilai Tnow: " << tnow << endl;
    cout << "Nilai Alpha: " << alpha << endl;

    /*
     * Generate CSV to Array
     */
    csv csv;
    if(csv.process())
    {
        cout << "===================***===================" << endl;
        memcpy(demand, csv.demand, sizeof(csv.demand));
        memcpy(jarak, csv.jarak, sizeof(csv.jarak));

        /* Random Rute Generate */
        rute_default[0] = 0;
        for(i = 1; i <= jml_rute; i++)
        {
            rute_default[i] = i;
        }

        /* VRSPD */
        vrspd* obj_v = new vrspd;
        obj_v->set_jml_rute(jml_rute);
        obj_v->set_jarak(jarak);
        obj_v->set_demand(demand);
        obj_v->set_quantity(quantity);

        /* Bangkitkan populasi Awal dan Evaluasi */
        for(k = 0; k < population; k++)
        {
            random_shuffle(&rute_default[1], &rute_default[jml_rute+1]);
            rute[k][0] = rute_default[0];

            //cout << "Rute " << k << ": ";
            //cout << rute_default[0] << " -> ";
            for(i = 1; i <= jml_rute; i++)
            {
                //cout << rute_default[i] << " -> ";
                rute[k][i] = rute_default[i];
            }
            //cout << rute_default[0];

            obj_v->set_rute(rute[k]);
            obj_v->calculate();
            f0[k] = obj_v->get_f0();

            //cout << "; F0: " << f0[k] << endl;
            //cout << "-----------------------------" << endl;
        }

        /* GA */
        ga* obj_ga = new ga;
        obj_ga->pc = pc;
        obj_ga->pm = pm;
        obj_ga->population = population;
        obj_ga->gen = jml_rute;
        obj_ga->quantity = quantity;

        memcpy(obj_ga->individu, rute, sizeof(rute));
        memcpy(obj_ga->demand, demand, sizeof(demand));
        memcpy(obj_ga->jarak, jarak, sizeof(jarak));

        /* SA */
        sa* obj_sa = new sa;
        obj_sa->psa = psa;
        obj_sa->tmin = tmin;
        obj_sa->tnow = tnow;
        obj_sa->alpha = alpha;
        obj_sa->population = population;
        obj_sa->gen = jml_rute;
        obj_sa->quantity = quantity;
        memcpy(obj_sa->individu, rute, sizeof(rute));
        memcpy(obj_sa->demand, demand, sizeof(demand));
        memcpy(obj_sa->jarak, jarak, sizeof(jarak));

        /* Iterasi */
        for(i = 0; i < iteration; i++)
        {
            cout << "************** Iterasi ke " << i << " ****************" << endl;
            elitism(population);
            //view_best_species(jml_rute, population);

            if(hibrid == true)
            {
                if(random_bool())
                {
                    cout << "GA" << endl;
                    obj_ga->pob = point_of_best;
                    memcpy(obj_ga->individu, rute, sizeof(rute));
                    //obj_ga->show_individu();
                    memcpy(obj_ga->ft, fitness, sizeof(fitness));
                    memcpy(obj_ga->ft_r, fitness_relatif, sizeof(fitness_relatif));
                    memcpy(obj_ga->ft_k, fitness_kumulatif, sizeof(fitness_kumulatif));

                    obj_ga->run();

                    for(k = 0; k < population; k++)
                    {
                        for(l = 0; l <= jml_rute; l++)
                        {
                            rute[k][l] = obj_ga->new_individu[k][l];
                        }
                        f0[k] = obj_ga->f0[k];
                    }

                    //obj_ga.show_individu_baru();

                }
                else
                {
                    cout << "SA" << endl;
                    obj_sa->pob = point_of_best;
                    memcpy(obj_sa->individu, rute, sizeof(rute));
                    //obj_sa->show_individu();
                    memcpy(obj_sa->ft, fitness, sizeof(fitness));
                    memcpy(obj_sa->ft_r, fitness_relatif, sizeof(fitness_relatif));
                    memcpy(obj_sa->ft_k, fitness_kumulatif, sizeof(fitness_kumulatif));

                    obj_sa->run();

                    for(k = 0; k < population; k++)
                    {
                        for(l = 0; l <= jml_rute; l++)
                        {
                            rute[k][l] = obj_sa->new_individu[k][l];
                        }
                        f0[k] = obj_sa->f0[k];
                    }
                }
            }
            else
            {
                cout << "GA" << endl;
                obj_ga->pob = point_of_best;
                memcpy(obj_ga->individu, rute, sizeof(rute));
                //obj_ga.show_individu();
                memcpy(obj_ga->ft, fitness, sizeof(fitness));
                memcpy(obj_ga->ft_r, fitness_relatif, sizeof(fitness_relatif));
                memcpy(obj_ga->ft_k, fitness_kumulatif, sizeof(fitness_kumulatif));

                obj_ga->run();

                for(k = 0; k < population; k++)
                {
                    for(l = 0; l <= jml_rute; l++)
                    {
                        rute[k][l] = obj_ga->new_individu[k][l];
                    }
                    f0[k] = obj_ga->f0[k];
                }
                cout << endl;

                //obj_ga.show_individu_baru();*/
            }
        }
        elitism(population);
        view_best_species(jml_rute, population);
    }
    return 0;
}
