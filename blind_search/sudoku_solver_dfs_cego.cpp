#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

int N;
unsigned long long calls;
vector<vector<int>> *sudoku;

void print_sudoku() {
    cout << endl << "SUDOKU:" << endl;
    for (int x = 0; x < N*N; x++) {
        for (int y = 0; y < N*N; y++) {
            cout << (*sudoku)[x][y] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool check_line(const int x) {
    unordered_set<int> s;
    
    for (int y = 0; y < N*N; y++) {
        if ((*sudoku)[x][y] != 0) s.insert((*sudoku)[x][y]);
    } 
    
    // cout << "Linha: " << (x+1) << endl;
    // cout << "Size: " << s.size() << endl;
    
    return s.size() == N*N;
}

bool check_column(const int y) {
    unordered_set<int> s;
    
    for (int x = 0; x < N*N; x++) {
        if ((*sudoku)[x][y] != 0) s.insert((*sudoku)[x][y]);
    }
    
    // cout << "Coluna: " << (y+1) << endl;
    // cout << "Size: " << s.size() << endl;
    
    return s.size() == N*N;
}

bool check_region(const int x, const int y) {
    unordered_set<int> s;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if ((*sudoku)[x+i][y+j] != 0) s.insert((*sudoku)[x+i][y+j]);
        }
    }
    
    // cout << "Regiao: (" << (x) << "," << (y) << ")" << endl;
    // cout << "Size: " << s.size() << endl;
    
    return s.size() == N*N;
}

bool check_solve() {
    bool check;
    
    for (int x = 0; x < N*N; x++) {
        check = check_line(x);
        if (check == false) return false;
    }
    
    for (int y = 0; y < N*N; y++) {
        check = check_column(y);
        if (check == false) return false;
    }
    
    for (int x = 0; x < N*N; x += N) {
        for (int y = 0; y < N*N; y += N) {
            check = check_region(x, y);
            if (check == false) return false;
        }
    }
    
    return true;
}

bool dfs(int x, int y, int pos) {
    bool ans;
    
    calls++;
    (*sudoku)[x][y] = pos;
    
    if(calls%100000 == 0) cout << "Chamadas: " << calls << endl;
    // print_sudoku();
    
    if (x == (N*N)-1 && y == (N*N)-1) {
        if (check_solve()) {
            cout << "Chamadas finais: " << calls << endl;
            print_sudoku();
            return true;
        }
        return false;
    }
    
    else if (y == (N*N)-1) {
        if ((*sudoku)[x+1][0] == 0) {
            for (int i = 1; i <= N*N; i++) {
                ans = dfs(x+1, 0, i);
                if (ans == true) return true;
            }
            (*sudoku)[x+1][0] = 0;
        }
        else {
            return dfs(x+1, 0, (*sudoku)[x+1][0]);
        }
    }
    else {
        if ((*sudoku)[x][y+1] == 0) {
            for (int i = 1; i <= N*N; i++) {
                ans = dfs(x, y+1, i);
                if (ans == true) return true;
            }
            (*sudoku)[x][y+1] = 0;
        }
        else {
            return dfs(x, y+1, (*sudoku)[x][y+1]);
        }
    }
    
    return false;
}

bool solve() {
    bool ans;
    
    if ((*sudoku)[0][0] == 0) {
        for (int i = 1; i <= N*N; i++) {
            ans = dfs(0, 0, i);
            if (ans == true) return true;
        }
        
        return false;
    }
    else {
        return dfs(0, 0, (*sudoku)[0][0]);
    }
}

int main() {
    int tmp;
    cin >> N;
    
    sudoku = new vector<vector<int>>(N*N, vector<int>(N*N, 0));
    
    for (int x = 0; x < N*N; x++) {
        for (int y = 0; y < N*N; y++) {
            cin >> tmp;
            (*sudoku)[x][y] = tmp;
        }
    }
    
    print_sudoku();
    
    if (solve() == false) {
        cout << endl << "Nao ha solucao" << endl;
    }
    
    
    delete sudoku;

    return 0;
}
