#ifndef CSV_HPP_INCLUDED
#define CSV_HPP_INCLUDED

using namespace std;

class csv
{
public:
    float jarak[MAX_ARR][MAX_ARR];
    float demand[MAX_ARR][MAX_ARR];
    csv()
    {

    }
    float * get_jarak()
    {
        return * this->jarak;
    }

    float * get_demand()
    {
        return * this->demand;
    }

    int parse_jarak()
    {
        int k = 0;
        vector<string> row;
        string line;

        //ifstream in("/home/ellaits/DataJarak.csv");
        ifstream in("DataJarak.csv");
        if (in.fail())
        {
            cout << "Jarak File not found" <<endl;
            return 0;
        } else {
            cout << "Jarak File ... OK" << endl;
        }

        while(getline(in, line)  && in.good() )
        {
            csvline_populate(row, line, ',');
            for(int i=0, leng=row.size(); i<leng; i++)
            {
                this->jarak[k][i] = strtof(row[i].c_str(), NULL);
            }
            k++;
        }
        in.close();

        return 1;
    }

    int parse_demand()
    {
        int k = 0;
        vector<string> row;
        string line;

        //ifstream in("/home/ellaits/DataDemand.csv");
        ifstream in("DataDemand.csv");
        //ifstream in("DataDemandSimulasiNormal.csv");
        //ifstream in("DataDemandSimulasiUniform.csv");
        if (in.fail())
        {
            cout << "Demand File not found" <<endl;
            return 0;
        } else {
            cout << "Demand File ... OK" << endl;
        }

        while(getline(in, line)  && in.good() )
        {
            csvline_populate(row, line, ',');
            for(int i=0, leng=row.size(); i<leng; i++)
            {
                this->demand[k][i] = strtof(row[i].c_str(), NULL);
            }
            k++;
        }
        in.close();

        return 1;
    }

    bool process()
    {
        cout << "============+++++++++++++++=============" << endl;
        cout << "============ Parse File CSV ============" << endl;
        cout << "=========================================" << endl;
        if(parse_demand()){
            if(parse_jarak()){
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
        cout << "===================***===================" << endl;
    }

    void csvline_populate(vector<string> &record, const string& line, char delimiter)
    {
        int linepos=0;
        int inquotes=false;
        char c;
        int linemax=line.length();
        string curstring;
        record.clear();

        while(line[linepos]!=0 && linepos < linemax)
        {

            c = line[linepos];

            if (!inquotes && curstring.length()==0 && c=='"')
            {
                inquotes=true;
            }
            else if (inquotes && c=='"')
            {
                if ( (linepos+1 <linemax) && (line[linepos+1]=='"') )
                {
                    curstring.push_back(c);
                    linepos++;
                }
                else
                {
                    inquotes=false;
                }
            }
            else if (!inquotes && c==delimiter)
            {
                record.push_back( curstring );
                curstring="";
            }
            else if (!inquotes && (c=='\r' || c=='\n') )
            {
                record.push_back( curstring );
                return;
            }
            else
            {
                curstring.push_back(c);
            }
            linepos++;
        }
        record.push_back( curstring );
        return;
    }

};

#endif // CSV_HPP_INCLUDED
