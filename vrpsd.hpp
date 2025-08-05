#ifndef VRPSD_HPP_INCLUDED
#define VRPSD_HPP_INCLUDED

#define MAX_ARR 255
#define CNT_DEMAND 5

using namespace std;

class vrspd
{
private:
    float jarak[MAX_ARR][MAX_ARR];
    float demand[MAX_ARR][MAX_ARR];
    int rute[MAX_ARR];
    int quantity;
    int jml_rute;
    int cnt_demand;
    float fr[MAX_ARR][MAX_ARR];
    float fp[MAX_ARR][MAX_ARR];
    float f[MAX_ARR][MAX_ARR];
    float h[MAX_ARR];

public:
    vrspd()
    {
        this->cnt_demand = 5;
        this->quantity = 0;
        this->jml_rute = MAX_ARR;
    }
	
	~vrspd(){}

    int count_demand(int data[MAX_ARR])
    {
        int count = 0;

        for(int k = 0; k < MAX_ARR; k++)
        {
            if(k == 0 && data[k] == 0)
            {
                count++;
            }
            if(data[k] != 0)
            {
                count++;
            }
        }

        return count;
    }

    void set_jml_rute(int jml_rute_input)
    {
        jml_rute = jml_rute_input;
    }

    void set_rute(int rute_input[MAX_ARR])
    {
        for(int i = 0; i < MAX_ARR; i++)
        {
            rute[i] = rute_input[i];
        }
    }

    void set_rute_vect(vector<int> rute_input)
    {
        /* for(int i = 0; i < MAX_ARR; i++)
        {
            rute[i] = &rute_input[i];
        } */
        cout << "Rute Vect: ";
        for(unsigned int i = 0; i < rute_input.size(); i++)
        {
            cout << rute_input[i] << ",";
            rute[i] = rute_input[i];
        }
    }

    void set_jarak(float jarak_input[MAX_ARR][MAX_ARR])
    {
        for(int i = 0; i < MAX_ARR; i++)
        {
            for(int k = 0; k < MAX_ARR; k++)
            {
                jarak[i][k] = jarak_input[i][k];
            }
        }
    }

    void set_demand(float demand_input[MAX_ARR][MAX_ARR])
    {
        for(int i = 0; i < MAX_ARR; i++)
        {
            for(int k = 0; k < MAX_ARR; k++)
            {
                demand[i][k] = demand_input[i][k];
            }
        }
    }

    void get_rute()
    {
        cout << " Rute :";
        for(int i = 0; i <= jml_rute; i++)
        {
            cout << "Rute ke " << i << " : " << rute[i] << endl;
        }
        cout << endl;
    }

    void set_quantity_view()
    {
        cout << "Quantity: ";
        cin >> quantity;
        cout << endl;
    }

    void set_quantity(int qty)
    {
        this->quantity = qty;
    }

    int get_quantity()
    {
        return quantity;
    }

    void calculate()
    {
        int cnt_demand_tmp = 0;

        for(int q = quantity; q >= 0; q--)
        {
            f[jml_rute][q] = jarak[rute[jml_rute]][0];
        }

        for(int j = (jml_rute - 1); j >= 0; j--)
        {
            //cnt_demand_tmp = this->count_demand(demand[rute[j+1]]);
            cnt_demand_tmp = 14;
            for(int q = quantity; q >= 0; q--)
            {
                fr[j][q] = jarak[rute[j]][0] + jarak[0][rute[j+1]] + this->sigma_quantity(cnt_demand_tmp, j);
            }
            for(int qp = quantity; qp >= 0; qp--)
            {
                fp[j][qp] = jarak[rute[j]][rute[j+1]] + this->sigma_jp(j, cnt_demand_tmp, qp);
                if(fr[j][qp] <= fp[j][qp])
                {
                    h[j] = qp;
                    f[j][qp] = fr[j][qp];
                }
                else if(fr[j][qp] > fp[j][qp])
                {
                    f[j][qp] = fp[j][qp];
                }
            }
        }

    }

    void get_result()
    {
        for(int j = jml_rute; j >= 0; j-- )
        {
            for(int i = quantity; i >= 0; i--)
            {
                cout << "F" << j << "(" << i << "):";
                cout << f[j][i] << ", ";
            }
            cout << endl;
            /* for(int i = quantity; i >= 0; i--)
            {
                cout << "Fr" << j << "(" << i << "):";
                cout << fr[j][i] << ", ";
            }
            cout << endl;*/
            /* for(int i = quantity; i >= 0; i--)
            {
                cout << "Fp" << j << "(" << i << "):";
                cout << fp[j][i] << ", ";
            }
            cout << endl;*/
            cout << "---";
            cout << endl;
        }
    }

    float get_f0()
    {
        float result[MAX_ARR];

        for ( int i = 0; i <= quantity; i++ )
        {
            int count = 0;
            int flag = 1;
            for ( int j = 0; j < i; j++ )
            {
                if ( f[0][i] == f[0][j] )
                    flag = flag + 1;
            }
            if ( flag == 1 )
            {
                for (int k = 0; k < quantity ; k++ )
                {
                    if ( f[0][i] == f[0][k] )
                        count = count + 1;
                }
                //cout << f[0][i] << " - " << count <<endl;
                result[i] = count;
            }
        }

        float tmp = 0;
        float f_distinct;
        for (int i = 0; i <= quantity; i++)
        {
            if(result[i] >= tmp)
            {
                tmp = result[i];
                f_distinct = f[0][i];
            }
        }

        return f_distinct;
    }

    float sigma_quantity(int k, int j)
    {
        int qk;
        float total = 0, pk;

        for(int i = 0; i < k; i++)
        {
            qk = demand[rute[j+1]][i];
            total = total + f[j+1][quantity-qk];
            // cout << "subtotal(" << j << ") : " << total << endl;
        }
        pk = demand[rute[j+1]][14];
        //total = total*pk;
        total = total/k;

        //cout << "total(" << j << ") : " << total << endl;

        return total;
    }

   float sigma_jp(int j, int k, int q)
    {
        float total = 0, tmp1, tmp2, pk;
        int b = 0, qk;

        pk = demand[rute[j+1]][14];
        for(int i = 0, m = 1; i < k; i++, m++)
        {
            qk = demand[rute[j+1]][i];
            if(qk <= q)
            {
                //total = total + (f[j+1][q-qk]*pk);
                total = total + (f[j+1][q-qk]/k);
            }
            else if(qk > q)
            {
                tmp1 = 2 * jarak[rute[j+1]][0];
                tmp2 = f[j+1][q+quantity-qk];
                //tmp2 = f[j+1][q+quantity-m];

                total = total + ((b + tmp1 + tmp2)/k);
            }

        }
        //cout << endl;

        return total;
    }
};

#endif // VRPSD_HPP_INCLUDED
