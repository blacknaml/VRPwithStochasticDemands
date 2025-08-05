#ifndef HELPER_HPP_INCLUDED
#define HELPER_HPP_INCLUDED

bool random_bool()
{
    return rand() % 2 == 1;
}

void elitism(int population)
{
    int k;
    float tmp_max, tmp_min;
    float o_tmp_min;

    point_of_best = 0;
    fitness_total = 0;
    tmp_min = tmp_max = f0[0];

    for(k = 1; k < population; k++)
    {
        if(f0[k] < tmp_min)
        {
            tmp_min = f0[k];
            point_of_best = k;
        }
        if(f0[k] > tmp_max)
        {
            tmp_max = f0[k];
        }
    }
    f0_best = tmp_min;
    memcpy(best_rute, rute[point_of_best], sizeof(rute[point_of_best]));

    o_tmp_min = f0[0];
     for(k = 1; k < population; k++)
    {
        if(k != point_of_best){
        if(f0[k] < o_tmp_min)
        {
            o_tmp_min = f0[k];
        }
        }
    }

    float total_min_max = o_tmp_min + tmp_max;
    for(k = 0; k < population; k++)
    {
        if(k != point_of_best)
        {
            fitness[k] =total_min_max - f0[k];
            fitness_total += fitness[k];
        }
    }

    float tmp_kumulatif = 0;
    for(k = 0; k < population; k++)
    {
        if(k != point_of_best)
        {
            fitness_relatif[k] = fitness[k] / fitness_total;
            tmp_kumulatif += fitness_relatif[k];
            fitness_kumulatif[k] = tmp_kumulatif;
        }
    }

}

void view_best_species(int jml_rute, int population)
{
    cout << "=========Terbaik=========" << endl;
    for(int j = 0; j <= jml_rute; j++)
    {
        cout << best_rute[j] << " -> ";
    }
    cout << "; F0: " << f0_best << endl;

    /* cout << endl;
    for(int k = 0; k < population; k++)
    {
        if(k != point_of_best)
        {
            cout << k << ". " << f0[k];
            cout << ", " << fitness[k];
            cout << ", " << fitness_relatif[k];
            cout << ", " << fitness_kumulatif[k] << endl;
        }
    } */
    cout << best_rute[0] << endl << "--------" << endl;
}

#endif // HELPER_HPP_INCLUDED
