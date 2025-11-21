#include <bits/stdc++.h>
#include <filesystem>
using namespace std;

struct BSJ {
    string readname;
    string chr;
    int start, end;

    bool operator==(const BSJ &other) const {
        return chr == other.chr && start == other.start && end == other.end;
    }
};

struct BSJHash {
    size_t operator()(const BSJ &b) const {
        return hash<string>()(b.chr) ^ hash<int>()(b.start) ^ hash<int>()(b.end);
    }
};

vector<BSJ> load_bsj_file(const string &path) {
    vector<BSJ> res;
    ifstream fin(path);
    if (!fin.is_open()) { cerr << "Cannot open file: " << path << endl; exit(1); }
    string line;
    getline(fin, line);
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        BSJ x;
        ss >> x.readname >> x.chr >> x.start >> x.end;
        res.push_back(x);
    }
    return res;
}

vector<BSJ> load_circbase(const string &path) {
    vector<BSJ> res;
    ifstream fin(path);
    if (!fin.is_open()) { cerr << "Cannot open file: " << path << endl; exit(1); }
    string line;
    unordered_set<BSJ, BSJHash> seen;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        BSJ x;
        ss >> x.chr >> x.start >> x.end;
        if (seen.count(x)) continue;
        seen.insert(x);
        res.push_back(x);
    }
    return res;
}

vector<BSJ> load_isocirc(const string &path) {
    vector<BSJ> res;
    ifstream fin(path);
    if (!fin.is_open()) { cerr << "Cannot open file: " << path << endl; exit(1); }
    string line;
    unordered_set<BSJ, BSJHash> seen;
    while (getline(fin, line)) {
        if (line.empty() || line[0]=='#') continue;
        stringstream ss(line);
        BSJ x;
        string isoformID;
        ss >> isoformID >> x.chr >> x.start >> x.end;
        if (x.chr.rfind("chr",0)!=0) x.chr = "chr" + x.chr;
        if (seen.count(x)) continue;
        seen.insert(x);
        res.push_back(x);
    }
    return res;
}

bool is_match(const BSJ &a, const BSJ &b, int tol) {
    if (a.chr != b.chr) return false;
    return abs(a.start-b.start)<=tol && abs(a.end-b.end)<=tol;
}

void print_samples(const string &name, const vector<BSJ> &v) {
    cerr << "[DBG] Loaded " << v.size() << " entries from " << name << endl;
    int n = min((int)v.size(), 3);
    for (int i=0;i<n;i++)
        cerr << "   " << v[i].chr << "\t" << v[i].start << "\t" << v[i].end << endl;
}

string get_basename(const string &path) {
    string name = path.substr(path.find_last_of("/\\")+1);
    size_t dot = name.find_last_of('.');
    if (dot!=string::npos) name = name.substr(0,dot);
    return name;
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Usage: ./compare <cirplex1.tsv> <cirplex2.tsv> ... <isocirc_annotation.tsv> <circbase_file> <output_dir>" << endl;
        return 1;
    }

    int BP_TOLERANCE = 50;

    // NEW --------------------------
    string isocirc_path = argv[argc-3];
    string circbase_path = argv[argc-2];   // now provided by user
    string output_dir    = argv[argc-1];
    // --------------------------------

    filesystem::create_directories(output_dir);

    cerr << "[i] Loading circBase..." << endl;
    auto circbase = load_circbase(circbase_path);
    print_samples("circBase", circbase);

    cerr << "[i] Loading IsoCirc annotation..." << endl;
    auto isocirc = load_isocirc(isocirc_path);
    print_samples("IsoCirc", isocirc);

    auto match_set = [&](const BSJ &x, const vector<BSJ> &pool){
        for(auto &y: pool) if(is_match(x,y,BP_TOLERANCE)) return true;
        return false;
    };

    for(int f=1; f<argc-3; f++){
        string cirplex_path = argv[f];
        string prefix = get_basename(cirplex_path);
        cerr << "\n[i] Processing " << prefix << endl;

        auto cirplex = load_bsj_file(cirplex_path);
        print_samples("cirplex", cirplex);

        vector<BSJ> cirplex_only, circbase_only, isocirc_only;
        vector<BSJ> cirplex_and_circbase, cirplex_and_isocirc, circbase_and_isocirc;
        vector<BSJ> cirplex_and_circbase_and_isocirc;

        for(auto &x: cirplex){
            bool inB = match_set(x, circbase);
            bool inC = match_set(x, isocirc);
            if(inB && inC) cirplex_and_circbase_and_isocirc.push_back(x);
            else if(inB) cirplex_and_circbase.push_back(x);
            else if(inC) cirplex_and_isocirc.push_back(x);
            else cirplex_only.push_back(x);
        }

        for(auto &x: isocirc){
            bool inA = match_set(x, cirplex);
            bool inB = match_set(x, circbase);
            if(!inA && inB) circbase_and_isocirc.push_back(x);
            else if(!inA && !inB) isocirc_only.push_back(x);
        }

        for(auto &x: circbase){
            bool inA = match_set(x, cirplex);
            bool inC = match_set(x, isocirc);
            if(!inA && !inC) circbase_only.push_back(x);
        }

        // auto write_vec = [&](const string &name, const vector<BSJ> &v){
        //     ofstream fout(output_dir+"/"+prefix+"_"+name+".tsv");
        //     fout << "chr\tstart\tend\n";
        //     for(auto &x: v) fout << x.chr<<"\t"<<x.start<<"\t"<<x.end<<"\n";
        // };

        // write_vec("cirplex_only", cirplex_only);
        // write_vec("circbase_only", circbase_only);
        // write_vec("isocirc_only", isocirc_only);
        // write_vec("cirplex_and_circbase", cirplex_and_circbase);
        // write_vec("cirplex_and_isocirc", cirplex_and_isocirc);
        // write_vec("circbase_and_isocirc", circbase_and_isocirc);
        // write_vec("cirplex_and_circbase_and_isocirc", cirplex_and_circbase_and_isocirc);

        ofstream vennstats(output_dir+"/"+prefix+"_venn_counts.txt");
        vennstats << "Region\tCount\n";
        vennstats << "Total_cirplex\t" << cirplex.size() << "\n";
        vennstats << "Total_circbase\t" << circbase.size() << "\n";
        vennstats << "Total_isocirc\t" << isocirc.size() << "\n";
        vennstats << "cirplex_only\t" << cirplex_only.size() << "\n";
        vennstats << "circbase_only\t" << circbase_only.size() << "\n";
        vennstats << "isocirc_only\t" << isocirc_only.size() << "\n";
        vennstats << "cirplex_and_circbase\t" << cirplex_and_circbase.size() << "\n";
        vennstats << "cirplex_and_isocirc\t" << cirplex_and_isocirc.size() << "\n";
        vennstats << "circbase_and_isocirc\t" << circbase_and_isocirc.size() << "\n";
        vennstats << "cirplex_and_circbase_and_isocirc\t" << cirplex_and_circbase_and_isocirc.size() << "\n";
        vennstats.close();

        cerr << "[✓] Done processing " << prefix << endl;
    }

    cerr << "\n[✓] All files processed. Outputs written in " << output_dir << endl;
    return 0;
}
