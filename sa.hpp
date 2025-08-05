#ifndef SA_HPP_INCLUDED
#define SA_HPP_INCLUDED

using namespace std;

class sa
{
private:
	vrspd* obj_v;
	vrspd* obj_vr;
    //int parent[255][255];
    int child_count, parent_count;
    vector< vector<int> > parent;

    void prospective_parent()
    {
        int i, k, m;
        vector<int> pros_parent(1000);
        int pros_parent_count = 0;
        float rand_bil;

        //cout << "Calon Induk: " << endl;
        for(i = 0; i < this->population; i++)
        {
            if(i != this->pob)
            {
                rand_bil = ((double) rand() / (RAND_MAX));
                for(k = 0; k < this->population; k++)
                {
                    if(rand_bil <= this->ft_k[k])
                    {
                        //cout << k << ", ";
                        pros_parent[i] = k;
                        break;
                    }
                    if((this->ft_k[k] < rand_bil) && (rand_bil <= this->ft_k[k+1]))
                    {
                        //cout << k << ", ";
                        pros_parent[i] = k;
                        break;
                    }
                }
                pros_parent_count++;
            }
        }
        //cout << endl << "========================" << endl;

        for(int i = 0; i < pros_parent_count; i++)
        {
            vector<int> row;
            this->parent.push_back(row);
        }

        //cout << "Calon Induk Psa: " << pros_parent_count << endl;
        for(i = 0, k = 0; i < pros_parent_count; i++)
        {
            //cout << "K : " << i;
            rand_bil = ((double) rand() / (RAND_MAX));
            if(rand_bil < this->psa)
            {
                for(m = 0; m <= this->gen; m++)
                {
                    //this->parent[k][m] = this->individu[pros_parent[i]][m];
                    this->parent[k].push_back(this->individu[pros_parent[i]][m]);
                }
                k++;
                //cout << pros_parent[i] << ", ";
            }
        }
        this->parent_count = k;
        //cout << endl << "========================" << endl;
    }
    void mutation()
    {
        int i, j, pc1, pc2, max, min;
        vector<int> tmp(1000);

        min = (int) this->gen / 2;
        max = (min+2);

        //cout << endl << "MUTASI" << endl;
        for(i = 0; i < this->parent_count; i++)
        {
            //cout << "Induk ke-" << i << ": ";
            /* salin individu parent ke tmp, untuk di acak dua titik nya */
            for(j = 0; j <= this->gen; j++)
            {
                tmp[j] = this->parent[i][j];
                //cout << tmp[j] << ", ";
            }
            //cout << endl;

            /* menentukan dua titik */
            pc1 = 1 + (rand() % (int)(min - 1 + 1));
            pc2 = max + (rand() % (int)(this->gen - max + 1));

            //cout << "p1 " << pc1 << ", p2 " << pc2 << endl;

            /* acak dua titik dari tmp */
            random_shuffle(&tmp[pc1], &tmp[pc2]);

            //cout << "Mutasi: ";
            for(j = 0; j <= this->gen; j++)
            {
                this->child[i][j] = tmp[j];
                //cout << tmp[j] << ", ";
            }
            //cout << endl << "---------------------" << endl;

        }
    }
    void regeneration()
    {
        int i, j, k, m;
        vector< vector<int> > tmp_individu(1000, vector<int>(1000));
        vector<int> buffer_individu(1000);

        float tmp_max, tmp_min, fitness_total = 0;
        vector<float> fitness(1000), fitness_relatif(1000), fitness_kumulatif(1000);

        for(i = 0, k = 0; i < this->population; i++)
        {
            if(i != this->pob)
            {
                for(j = 0; j <= this->gen; j++)
                {
                    tmp_individu[k][j] = this->individu[i][j];
                }
                k++;
            }
        }
        for(i = 0; i < this->parent_count; i++)
        {
            if(i != this->sa_pob)
            {
                for(j = 0; j <= this->gen; j++)
                {
                    tmp_individu[k][j] = this->parent[i][j];
                }
                k++;
            }
        }

        //cout << "K value: " << k << endl;

        /* Fungsi tujuan vrpsd */
        int tmp_rute_vect[MAX_ARR];

        obj_v = new vrspd;
        obj_v->set_jml_rute(this->gen);
        obj_v->set_jarak(this->jarak);
        obj_v->set_demand(this->demand);
        obj_v->set_quantity(this->quantity);

        //cout << "____________________________" << endl;
        //cout << endl << "POPULASI MUTASI + LAMA" << endl;
        for(i = 0; i < k; i++)
        {
            //obj_v.set_rute_vect(tmp_individu[i]);
            for(m = 0; m <= gen; m++)
            {
                tmp_rute_vect[m] = tmp_individu[i][m];
            }
            obj_v->set_rute(tmp_rute_vect);
            obj_v->calculate();
            f0[i] = obj_v->get_f0();
            /* cout << "individu ke-" << i << endl;
            for(j = 0; j <= this->gen; j++)
            {
                cout << tmp_individu[i][j] << " -> ";
            } */

            //cout << "; F0: " << f0[i] << endl;
        }
        //cout << "_____________________________" << endl;

        /* Fitness */
        tmp_min = tmp_max = f0[0];
        for(i = 1; i < k; i++)
        {
            if(f0[i] < tmp_min)
            {
                tmp_min = f0[i];
            }
            if(f0[i] > tmp_max)
            {
                tmp_max = f0[i];
            }
        }

        //cout << "TMP MIN: " << tmp_min << "TMP MAX" << tmp_max;

        float total_min_max = tmp_min + tmp_max;
        for(i = 0; i < k; i++)
        {
            fitness[i] = total_min_max - f0[i];
            fitness_total += fitness[i];
        }

        //cout << "Fitness Kumulatif: " << endl;
        //cout << " -------------------- " << endl;

        float rand_bil;
        float tmp_kumulatif = 0;
        for(i = 0; i < k; i++)
        {
            fitness_relatif[i] = fitness[i] / fitness_total;
            tmp_kumulatif += fitness_relatif[i];
            fitness_kumulatif[i] = tmp_kumulatif;
            //cout << fitness_kumulatif[i] << " -> ";
        }
        /* End Fitness */

        //cout << "Seleksi Individu" << endl;
        for(i = 0; i < (this->population - 2); i++)
        {
            rand_bil = ((double) rand() / (RAND_MAX));
            for(j = 0; j < k; j++)
            {
                if(rand_bil <= fitness_kumulatif[j])
                {
                    //cout << j << ", ";
                    for(m = 0; m <= this->gen; m++)
                    {
                        this->new_individu[i][m] = tmp_individu[j][m];
                    }
                    break;
                }
                if((fitness_kumulatif[j] < rand_bil) && (rand_bil <= fitness_kumulatif[j+1]))
                {
                    //cout << j << ", ";
                    for(m = 0; m <= this->gen; m++)
                    {
                        this->new_individu[i][m] = tmp_individu[j][m];
                    }
                    break;
                }
            }
        }

        //cout << endl;
        for(m = 0; m <= this->gen; m++)
        {
            this->new_individu[i][m] = this->individu[this->pob][m];
        }
        i++;
        for(m = 0; m <= this->gen; m++)
        {
            this->new_individu[i][m] = this->parent[this->sa_pob][m];
        }

        //cout << "===================================================" << endl;
        //cout << "POPULASI BARU" << endl;
        for(i = 0; i < this->population; i++)
        {
            obj_v->set_rute(this->new_individu[i]);
            obj_v->calculate();
            f0[i] = obj_v->get_f0();

            //cout << "Individu ke-" << i << endl;
            //for(m =0; m <= this->gen; m++)
            //{
              //  cout << this->new_individu[i][m] << "->";
            //}
            //cout << endl << "; F0: " << f0[i] << endl;
        }
        //cout << "===================================================" << endl;

    }
public:
    int population, gen, pob, quantity, sa_pob;
    int individu[MAX_ARR][MAX_ARR];
    int new_individu[MAX_ARR][MAX_ARR];
    int child[255][255];

    float jarak[MAX_ARR][MAX_ARR];
    float demand[MAX_ARR][MAX_ARR];
    float f0[MAX_ARR], zk[MAX_ARR];

    float psa, tmin, tnow, alpha;
    float ft[MAX_ARR];
    float ft_r[MAX_ARR];
    float ft_k[MAX_ARR];

    sa()
    {
        this->psa = this->tmin = this->tnow = this->alpha = this->parent_count = 0;
    }

	~sa(){
		delete(obj_v);
		delete(obj_vr);
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
            cout << "; F0: " << this->f0[k];
            cout << endl;
        }
        cout << "--------------------------------" << endl;
    }
    void run()
    {
        float t, p, rand_bil, f0_p[255], f0_c[255];
        int i, j, m;
        int tmp_rute_vect[MAX_ARR];

        this->prospective_parent();
        this->mutation();

        /* Fungsi tujuan vrpsd */
        obj_vr = new vrspd;
        obj_vr->set_jml_rute(this->gen);
        obj_vr->set_jarak(this->jarak);
        obj_vr->set_demand(this->demand);
        obj_vr->set_quantity(this->quantity);

        t = this->tnow;
        i = 1;
        do
        {
            cout << "************************" << endl;
            cout << "T: " << t << ", tmin: " << this->tmin << endl;

            for(j = 0; j < this->parent_count; j++)
            {

                for(m = 0; m <= gen; m++)
                {
                    tmp_rute_vect[m] = this->parent[j][m];
                }
                obj_vr->set_rute(tmp_rute_vect);
                obj_vr->calculate();
                f0_p[j] = obj_vr->get_f0();

                obj_vr->set_rute(this->child[j]);
                obj_vr->calculate();
                f0_c[j] = obj_vr->get_f0();

                if(f0_c[j] <= f0_p[j])
                {
                    for(m = 0; m <= this->gen; m++)
                    {
                        this->parent[j][m] = this->child[j][m];
                    }
                    this->zk[j] = f0_c[j];
                }
                else
                {
                    rand_bil = ((double) rand() / (RAND_MAX));
                    p = exp((-1*((f0_c[j]-f0_p[j])/100)));
                    if(p > rand_bil)
                    {
                        for(m = 0; m <= this->gen; m++)
                        {
                            this->parent[j][m] = this->child[j][m];
                        }
                        this->zk[j] = f0_c[j];
                    }
                    else
                    {
                        this->zk[j] = f0_p[j];
                    }
                }
            }

            t = pow(this->alpha, i)*this->tnow;

            if(t > this->tmin)
            {
                this->mutation();
            }

            i++;

            cout << "************************" << endl;
        } while((int)t > (int) this->tmin);

        float f0p_min = zk[0];
        for(j = 1; j < this->parent_count; j++)
        {
            if(zk[j] < f0p_min)
            {
                f0p_min = this->zk[j];
                this->sa_pob = j;
            }
        }

        //cout << "BEST SA" << this->sa_pob << ": " << zk[this->sa_pob];
        this->regeneration();

    }
};

#endif // SA_HPP_INCLUDED
