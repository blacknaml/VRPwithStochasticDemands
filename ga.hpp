#ifndef GA_HPP_INCLUDED
#define GA_HPP_INCLUDED

using namespace std;

class ga
{
private:
    vrspd* obj_vs;
    int i, child_key;
    float rand_r[MAX_ARR];
    float rand_k[MAX_ARR];
    int tmp_individu[MAX_ARR][MAX_ARR];
    int crossover_parent_key[MAX_ARR], mutation_parent_key[MAX_ARR];
    int crossover_parent_cnt, mutation_parent_cnt;

    bool check_crossover_parent(int max_iter, int value)
    {
        int i;
        bool is_ok = true;
        for(i = 0; i < (max_iter+1); i++)
        {
            if(this->crossover_parent_key[i] == value)
            {
                is_ok = false;
            }
        }

        return is_ok;
    }

    bool check_mutation_parent(int max_iter, int value)
    {
        int i;
        bool is_ok = true;

        for(i = 0; i < (max_iter+1); i++)
        {
            if(this->mutation_parent_key[i] == value)
            {
                is_ok = false;
            }
        }
        return is_ok;
    }

    void crossover_parent_filter()
    {
        int j = 0, k;
        int crossover_parent_key_tmp[MAX_ARR];

        for(i = 0; i < this->population; i++)
        {
            this->rand_r[i] = ((double) rand() / (RAND_MAX));
            if(this->rand_r[i] <= ft_k[i])
            {
                if(this->check_crossover_parent(j, i) == true)
                {
                    crossover_parent_key_tmp[j] = i;
                    j++;

                }
            }
        }

        float tmp;
        for(i = 1; i < this->population; i++)
        {
            k = (i-1);
            tmp = this->ft_k[k];
            if((tmp < this->rand_r[k]) && (this->rand_r[k] <= this->ft_k[i]))
            {
                if(this->check_crossover_parent(j, k) == true)
                {
                    crossover_parent_key_tmp[j] = k;
                    j++;
                }
            }
        }

        float rand_tmp;
        this->crossover_parent_cnt = 0;
        for(i = 0; i < j; i++)
        {
            rand_tmp = ((double) rand() / (RAND_MAX));
            if(rand_tmp < this->pc)
            {
                this->crossover_parent_key[this->crossover_parent_cnt] = crossover_parent_key_tmp[i];
                this->crossover_parent_cnt++;
            }
        }

    }
    void mutation_parent_filter()
    {
        int j = 0, k;
        int mutation_parent_key_tmp[MAX_ARR];

        for(i = 0; i < this->population; i++)
        {
            this->rand_k[i] = ((double) rand() / (RAND_MAX));
            if(this->rand_k[i] <= ft_k[i])
            {
                if(this->check_mutation_parent(j, i) == true)
                {
                    mutation_parent_key_tmp[j] = i;
                    j++;
                }
            }
        }

        float tmp;
        for(i = 1; i < this->population; i++)
        {
            k = (i-1);
            tmp = this->ft_k[k];
            if((tmp < this->rand_k[k]) && (this->rand_k[k] <= this->ft_k[i]))
            {
                if(this->check_mutation_parent(j, k) == true)
                {
                    mutation_parent_key_tmp[j] = k;
                    j++;
                }
            }
        }

        float rand_tmp;
        this->mutation_parent_cnt = 0;
        for(i = 0; i < j; i++)
        {
            rand_tmp = ((double) rand() / (RAND_MAX));
            if(rand_tmp < this->pm)
            {
                this->mutation_parent_key[this->mutation_parent_cnt] = mutation_parent_key_tmp[i];
                this->mutation_parent_cnt++;
            }
        }
    }
    void crossover()
    {
        int i, j, k, l, m, max, min, mod, max_iter;
        int pc1, pc2;
        int buffer[MAX_ARR], indeks_buffer[MAX_ARR];

        mod = this->crossover_parent_cnt % 2;
        if(mod > 0)
        {
            max_iter = (this->crossover_parent_cnt - 1);
        }
        else
        {
            max_iter = this->crossover_parent_cnt;
        }

        min = (int) this->gen / 2;
        max = (min+1);

        for(i = 0; i < max_iter; i+=2)
        {
            pc1 = 1 + (rand() % (int)(min - 1 + 1));
            pc2 = max + (rand() % (int)(this->gen - max + 1));

            for(k = 0, j = 0; k <= this->gen; k++)
            {
                child[i][k] = individu[crossover_parent_key[i]][k];
                child[i+1][k] = individu[crossover_parent_key[i+1]][k];
                if(k <= pc1 || k >= pc2)
                {
                    buffer[j] = individu[crossover_parent_key[i]][k];
                    indeks_buffer[j] = k;
                    j++;
                }
            }

            m = 0;
            for(k = 0; k <= this->gen; k++)
            {
                for(l = 0; l < j; l++)
                {
                    if(buffer[l] == individu[crossover_parent_key[i+1]][k])
                    {
                        child[i+1][k] = buffer[m];
                        child[i][indeks_buffer[m]] = individu[crossover_parent_key[i+1]][k];
                        m++;
                    }
                }
            }
        }
        this->child_key = max_iter;

        //cout << "---------------- CX CHILD ------------------" << endl;
        for(i = 0; i < max_iter; i++)
        {
            for(k = 0; k <= this->gen; k++)
            {
                //cout << child[i][k] << " -> ";
            }
            //cout << endl;
        }
        //cout << "---------------- END CHILD ----------------" << endl;
    }
    void mutation()
    {
        int i, pc1, pc2, max, min, p1, p2;

        min = (int) this->gen / 2;
        max = (min+1);

        for(i = 0; i < mutation_parent_cnt; i++)
        {
            pc1 = 1 + (rand() % (int)(min - 1 + 1));
            pc2 = max + (rand() % (int)(this->gen - max + 1));

            p1 = individu[mutation_parent_key[i]][pc1];
            p2 = individu[mutation_parent_key[i]][pc2];
            memcpy(this->child[this->child_key], individu[mutation_parent_key[i]], sizeof(individu[mutation_parent_key[i]]));
            child[this->child_key][pc1] = p2;
            child[this->child_key][pc2] = p1;

            this->child_key++;
        }
    }
    void regeneration()
    {
        int i, j, k, m;

        float tmp_max, tmp_min, fitness_total = 0;
        float fitness[MAX_ARR];
        float fitness_relatif[MAX_ARR];
        float fitness_kumulatif[MAX_ARR];

        for(i = 0, j = 0; i < this->population; i++)
        {
            if(i != this->pob)
            {
                memcpy(this->tmp_individu[j], this->individu[i], sizeof(this->individu[i]));
                j++;
            }
        }
        for(i = 0; i < this->child_key; i++)
        {
            memcpy(this->tmp_individu[j], this->child[i], sizeof(this->child[i]));
            j++;
        }

        /* VRPSD */
        obj_vs = new vrspd;
        obj_vs->set_jml_rute(this->gen);
        obj_vs->set_jarak(this->jarak);
        obj_vs->set_demand(this->demand);
        obj_vs->set_quantity(this->quantity);

        //cout << "Individu Baru" << endl;
        for(i = 0; i < j; i++)
        {

            obj_vs->set_rute(tmp_individu[i]);
            obj_vs->calculate();
            f0[i] = obj_vs->get_f0();
            /* for(m = 0; m <= this->gen; m++)
            {
                cout << tmp_individu[i][m] << " -> ";
            }*/

            //cout << "; F0: " << f0[i] << endl;
        }
        //cout << "-----------------------------" << endl;

        /* Fitness */
        tmp_min = tmp_max = f0[0];
        for(k = 1; k < j; k++)
        {
            if(f0[k] < tmp_min)
            {
                tmp_min = f0[k];
            }
            if(f0[k] > tmp_max)
            {
                tmp_max = f0[k];
            }
        }

        float total_min_max = tmp_min + tmp_max;
        for(k = 0; k < j; k++)
        {
            fitness[k] =total_min_max - f0[k];
            fitness_total += fitness[k];
        }

        float rand_bil;
        float tmp_kumulatif = 0;
        for(k = 0; k < j; k++)
        {
            fitness_relatif[k] = fitness[k] / fitness_total;
            tmp_kumulatif += fitness_relatif[k];
            fitness_kumulatif[k] = tmp_kumulatif;
        }
        /* End Fitness */

        for(i = 0; i < (this->population - 1); i++)
        {
            rand_bil = ((double) rand() / (RAND_MAX));
            for(k = 0; k < j; k++)
            {
                if(rand_bil <= fitness_kumulatif[k])
                {
                    for(m = 0; m <= this->gen; m++)
                    {
                        this->new_individu[i][m] = this->tmp_individu[k][m];
                    }
                    break;
                }
                if((fitness_kumulatif[k] < rand_bil) && (rand_bil <= fitness_kumulatif[k+1]))
                {
                    for(m = 0; m <= this->gen; m++)
                    {
                        this->new_individu[i][m] = this->tmp_individu[k][m];
                    }
                    break;
                }
            }
        }

        for(m = 0; m <= this->gen; m++)
        {
            this->new_individu[i][m] = this->individu[pob][m];
        }

        for(i = 0; i < this->population; i++)
        {
            obj_vs->set_rute(new_individu[i]);
            obj_vs->calculate();
            f0[i] = obj_vs->get_f0();
        }

    }

public:
    int population, gen, pob, quantity;
    int individu[MAX_ARR][MAX_ARR];
    int new_individu[MAX_ARR][MAX_ARR];
    int child[MAX_ARR][MAX_ARR];

    float jarak[MAX_ARR][MAX_ARR];
    float demand[MAX_ARR][MAX_ARR];
    float f0[MAX_ARR];

    float pc, pm;
    float ft[MAX_ARR];
    float ft_r[MAX_ARR];
    float ft_k[MAX_ARR];

    ga()
    {
        this->pc = this->pm = this->child_key = 0;
    }

    ~ga()
    {
        delete(obj_vs);
    }

    void show_individu()
    {
        int k, i;

        cout << "Populasi Sekarang" << endl;
        for(k = 0; k < this->population; k++)
        {
            cout << k << ": ";
            for(i = 1; i <= this->gen; i++)
            {
                cout << this->individu[k][i] << " -> ";
            }
            cout << endl;
        }
        cout << "--------------------------------" << endl;
    }

    void show_individu_baru()
    {
        int k, i;

        cout << "Populasi Baru" << endl;
        for(k = 0; k < this->population; k++)
        {
            cout << k << ": ";
            for(i = 1; i <= this->gen; i++)
            {
                cout << this->new_individu[k][i] << " -> ";
            }
            cout << endl;
        }
        cout << "--------------------------------" << endl;
    }

    void run()
    {
        this->crossover_parent_filter();
        this->mutation_parent_filter();
        this->crossover();
        this->mutation();
        this->regeneration();
    }
};

#endif // GA_HPP_INCLUDED


