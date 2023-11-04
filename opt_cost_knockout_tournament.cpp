#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <climits>

using namespace std;

struct VectorHash {
    size_t operator()(const vector<int>& v) const {
        size_t hash = 0;
        for (int i : v) {
            hash ^= std::hash<int>{}(i);
        }
        return hash;
    }
};
struct VectorEqual {
    bool operator()(const vector<int>& a, const vector<int>& b) const {
        return a == b;
    }
};

using VectorIntHashMap = unordered_map<vector<int>, int, VectorHash, VectorEqual>;
using VectorVectorHashMap = unordered_map<vector<int>, vector<int>, VectorHash, VectorEqual>;
VectorIntHashMap OptC;
VectorVectorHashMap OptS;

// Function to generate all subsets of size k from the set Sigma
void generateSubsets(const vector<int>& Sigma, int k, vector<int> currentSubset, int index, vector<vector<int>>& subsets) {
    if (currentSubset.size() == k) {
        // Add the valid subset to the vector
        subsets.push_back(currentSubset);
        return;
    }

    if (index == Sigma.size()) {
        return;
    }

    // Include the current element in the subset
    currentSubset.push_back(Sigma[index]);
    generateSubsets(Sigma, k, currentSubset, index + 1, subsets);

    // Exclude the current element from the subset
    currentSubset.pop_back();
    generateSubsets(Sigma, k, currentSubset, index + 1, subsets);
}

int OptTourCost(vector<int> sigma, int num, vector<int> q)
{
    int n=ceil(log2(num));
    int c_max;
    int temp1,temp2;

    vector<int> s_max;
    if(n==0)
    {
        c_max=0;
        //s_max is empty
    }
    else if(n==1)
    {
        c_max=q[sigma[0]-1]*q[sigma[1]-1];
        s_max.push_back(sigma[0]);
    }
    else
    {
        int k_min;
        int p1=pow(2,n-2);
        int p2=2*p1;
        int k_max=ceil(num/2);
        if(num<=p1+p2)
        {
            k_min=p1;
        }
        else
        {
            k_min=num-p2;
        }
        c_max=INT_MIN;
        for(int k=k_min;k<=k_max;k++)
        {
            vector<vector<int>> generatedSubsets;
            vector<int> currentSubset;
            generateSubsets(sigma, k, currentSubset, 0, generatedSubsets);
            for (const auto& sigma1 : generatedSubsets)
            {
                int c1,c2,c;
                // Check if the subset is present in OptC
                if (OptC.find(sigma1) == OptC.end())  // if not present
                {
                    c1=OptTourCost(sigma1,k,q);
                }

                else
                {
                    c1=OptC[sigma1];
                }
                vector<int> sigma2;
                for (int element : sigma)
                {
                    bool found = false;
                    for (int s1 : sigma1)
                    {
                        if (element == s1)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        sigma2.push_back(element);
                    }
                }
                if (OptC.find(sigma2) == OptC.end())  // if not present
                {
                    c2=OptTourCost(sigma2,k,q);
                }
                else
                {
                    c2=OptC[sigma2];
                }
                c=c1+c2;
                int ql = 0;  // Initialize ql to 0
                cout<<"\n\n\t\t------------------------------------------------------------------------------------------------------------------\n";
                cout<<"\n\t\t\t\t\t\t\tplayers of left subtree       --   ";
                for (int i : sigma1)
                {
                    cout<<i<<"  ";
                    ql += q[i - 1];  // Assuming player indices start from 1
                }
                int qr = 0;  // Initialize qr to 0
                cout<<"\n\t\t\t\t\t\t\tplayers of right subtree      --   ";
                for (int i : sigma2) {
                    cout<<i<<"  ";
                    qr += q[i - 1];  // Assuming player indices start from 1
                }
                c = c + (k * ql * qr);
                cout<<"\n\t\t\t\t\t\t\tquota of left subtree         --   "<<ql<<endl;
                cout<<"\t\t\t\t\t\t\tquota of right subtree        --   "<<qr<<endl;
                cout<<"\t\t\t\t\t\t\tcost of left subtree          --   "<<c1<<endl;
                cout<<"\t\t\t\t\t\t\tcost of right subtree         --   "<<c2<<endl;
                cout<<"\t\t\t\t\t\t\tcost of total subtree         --   "<<c<<endl;
                cout<<"\n\n\t\t------------------------------------------------------------------------------------------------------------------\n";
                if (c > c_max) {
                    c_max = c;
                    s_max = sigma1;
                }

            }
        }
    }
    OptC[sigma]=c_max;
    OptS[sigma]=s_max;
    return c_max;
}

int main() {

    //getting inputs
    int num_players;
    cout<<"\n\n\t\t------------------------------------------------------------------------------------------------------------------\n\n\n";
    cout<<"\t\t\t\t\t\t\t OPTIMAL KNOCKOUT TOURNAMENT ! \n";
    cout<<"\n\n\t\t------------------------------------------------------------------------------------------------------------------\n";
    cout<<"\n\n\t\tEnter number of players :";
    cin>>num_players;

    vector<int> sigma(num_players);
    vector<int> quota(num_players);

    for(int i=0;i<num_players;i++)
    {
        sigma[i]=i+1;
    }

    cout<<"\n\n\t\tQUOTATIONS OF PLAYERS: \n\n";
    for(int i=0; i<num_players;i++)
    {
        cout<<"\t\t\t\tEnter the quota of "<<sigma[i]<<": ";
        cin>>quota[i];
    }
    int result_cost = OptTourCost(sigma,num_players,quota);
    cout<<"\n\n\t\t\t\t\t\t\tOptimal cost : "<<result_cost<<endl;
    cout<<"\n\n\t\t------------------------------------------------------------------------------------------------------------------\n";
}