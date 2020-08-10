#include<bits/stdc++.h> 
using namespace std;

struct edge{
    vector<pair<int,int>> dependencies; 
};
bool non_conflicting(int x, int y, int ni,vector<vector<int> > &outs, vector<vector<int>> & nextst, vector<vector<bool>> &conflicting){
	//cout<<x<<" "<<y<<"!\n";
    for(int i = 0;i<ni; i++){
        if(nextst[x][i] != -1 && nextst[y][i] != -1){
            if(conflicting[nextst[x][i]][nextst[y][i]])
                return false;
        }
        //cout<<x<<" "<<y<<"@\n";
        if(outs[x][i] == -1 || outs[y][i] == -1)
        {
            continue;
        }
       // cout<<x<<" "<<y<<"#\n";
        if(outs[x][i] != outs[y][i])
            return false;
     //   cout<<x<<"! "<<y<<"\n";
    }
    return true;
}

void merge(int x, int y, int ni,vector<vector<int>> &outs, vector<vector<int>> & nextst, vector<vector<edge>> &graph){
    
    edge e;
    int chk=0;
    for(int i = 0; i<ni; i++){
      //  if(outs[x][i] == -1 || outs[y][i] == -1)
         //   continue;
        if(nextst[x][i] == nextst[y][i])
            continue;
        if(nextst[x][i]<0||nextst[y][i]<0)continue;
        int a = nextst[x][i];
        int b = nextst[y][i];
        if(a>b)
            swap(a,b);
        if(make_pair(a,b) == make_pair(x,y))
            continue;
        int ch=1;
        for(auto x:e.dependencies)
        {
        	pair<int,int> y={a,b};
        	if(x==y){ch=0; break;}
        }
        if(ch)e.dependencies.push_back(make_pair(a, b));
        chk++;
        //e.dependencies.push_back(make_pair(nextst[y][i], nextst[x][i]));
    }
    if(chk==0)
    {
    	e.dependencies.push_back({-100000, -100000});
    }
    graph[x][y] = e;
    graph[y][x]=e;
}

int main(){
    int ns,chk=1;
    cin>> ns;
    int ni;
    cin>>ni;
    int inp[ni];
    for(int i = 0;i<ni;i++){
        cin>>inp[i];
    }
    
    int no;
    cin>> no;
    int out[no];
    for(int i = 0; i<no; i++){
        cin>>out[i];
    }
    vector<vector<int> > nextst(ns,vector<int>(ni));
    vector<vector<int> > outs = nextst;
    for(int i = 0; i<ns; i++){
        for(int j = 0;j < ni; j++){
            cin>>nextst[i][j];
            if(nextst[i][j]>0)nextst[i][j]--;
            cin>>outs[i][j];
        }
    }
    vector<vector<edge> > graph(ns,vector<edge>(ns)),cgraph(ns,vector<edge>(ns)),rcgraph(ns,vector<edge>(ns));
    vector<vector<bool> > conflicting(ns,vector<bool>(ns,false));
    while(chk){
    	chk=0;
		for(int i = 0;i<ns;i++){
		    for(int j = i+1;j<ns;j++){
		    	//cout<<i<<" "<<j<<"\n";
		        if(non_conflicting(i,j,ni,outs, nextst, conflicting) == false&&conflicting[i][j]==false)
		             conflicting[i][j] = true, conflicting[j][i] = true,chk=1;        
		    }
		}
	}
    /*for(int i = 0;i<ns;i++){
        for(int j = i+1;j<ns;j++)cout<<conflicting[i][j]<<" ";
        cout<<"\n";
    }*/
    for(int i = 0;i<ns;i++){
        for(int j = i+1;j<ns;j++){
            if(conflicting[i][j]==false)
                merge(i,j,ni,outs,nextst,graph);            
        }
    }
    for(int i=0;i<ns;i++)
    {
    	for(int j=i+1;j<ns;j++)
    	{
    		edge e=graph[i][j];
    		for(auto x:e.dependencies)
    		{
    			cgraph[i][j].dependencies.push_back({x.first,x.second});
    			if(x.first>=0)rcgraph[x.first][x.second].dependencies.push_back({i,j});
    		}
    	}
    }
    cout<<"MERGER GRAPH\n";
	for(int i=0;i<ns;i++)
	{
		for(int j=i+1;j<ns;j++)
		{
			edge e=graph[i][j];
			if(e.dependencies.empty())continue;
			cout<<i<<" -> "<<j<<":\n";
			for(auto x:e.dependencies)if(x.first>=0)cout<<" "<<x.first<<" "<<x.second<<"\n";
		}
	}
	cout<<"COMPABILITY GRAPH\n";
	vector<pair<int,int> >v;
	for(int i=0;i<ns;i++)
	{
		for(int j=i+1;j<ns;j++)
		{
			//cout<<i<<" "<<j<<"\n";
			edge e=cgraph[i][j];
			if(e.dependencies.empty())continue;
			cout<<"("<<i<<","<<j<<")"<<": \n";
			v.push_back({i,j});
			for(auto x:e.dependencies)if(x.first>=0)cout<<" "<<x.first<<" "<<x.second<<"\n";
		}
	}
	int k=v.size(),mi=1e9,pos=-1,a1,a2,a3,an1,an2,an3;
	//for(auto x:v)cout<<x.first<<" "<<x.second<<"\n";
	int mask=1<<k;
	for(int i=0;i<mask;i++)
	{
		int p=1;
		map<pair<int,int>,int>m;
		map<int,int>m1;
		vector<int>g[ns];
		for(int j=0;j<k;j++)
		{
			p=1<<j;
			int yu=i&p;
			if(yu==0)continue;
			m[v[j]]++;
			m1[v[j].first]=1;
			g[v[j].first].push_back(v[j].second);
			m1[v[j].second]=1;
		}
		int cost=0,u,cyc=0;
		for(int j=0;j<ns;j++){if(m1[j]==0)cost++; }//else u=j;}
		for(auto q:m)cost++;
		if(i==283)
		{
			//cout<<cost<<"\n";
			//for(auto x:m)cout<<x.first.first<<" "<<x.first.second<<"\n";
			//for(auto x:m1)cout<<x.first<<"\n";
		}
		for(int x=0;x<ns;x++)
		{
			for(int y=x+1;y<ns;y++)
			{
				for(int z=y+1;z<ns;z++)
				{
					a1=m[{x,y}],a2=m[{x,z}],a3=m[{y,z}];
					if(a1&&a2&&a3){cyc=1; break;}
				}
				if(cyc)break;
			}
			if(cyc)break;
		}
		if(cyc)cost-=2;
		//if(i==283)cout<<cost<<"\n";
		if(mi>=cost){mi=cost; pos=i;}
	}
	//cout<<"!\n";
	cout<<mi<<" "<<pos<<"\n";
	int p=1;
	map<pair<int,int>,int>m;
	map<int,int>m1;
	map<pair<int,pair<int,int> >,int >m2;
	vector<int>g[ns];
	for(int j=0;j<k;j++)
	{
		p=1<<j;
		int uy=pos&p;
		if(uy==0)continue;
		m[v[j]]++;
		m1[v[j].first]=1;
		g[v[j].first].push_back(v[j].second);
		m1[v[j].second]=1;
	}
	int cost=0,u,cyc=0,x1;
	for(int j=0;j<ns;j++){if(m1[j]==0)cost++; }//else u=j;}
	for(auto q:m)cost++;
	for(int x=0;x<ns;x++)
	{
		for(int y=x+1;y<ns;y++)
		{
			for(int z=y+1;z<ns;z++)
			{
				a1=m[{x,y}],a2=m[{x,z}],a3=m[{y,z}];
				if(a1&&a2&&a3){cyc=1; an1=x,an2=y,an3=z; break;}
			}
			if(cyc)break;
		}
		if(cyc)break;
	}
	//if(cyc)cost-=2;
	//cout<<cost<<"\n";
	int count=1;
	if(cyc)
	{
		cost-=2;
		m[{an1,an2}]=0,m[{an1,an3}]=0,m[{an2,an3}]=0;
		//cout<<an1<<" "<<an2<<" "<<an3<<"\n";
		m2[{an1,{an2,an3}}]=1;
		count++;
	}
	for(auto x:m)
	{
		if(x.second==0)continue;
		m2[{-1,{x.first.first,x.first.second}}]=count;
		count++;
	}
	for(int i=0;i<ns;i++){if(m1[i]==0){m2[{-1,{-1,i}}]=count; count++;}}
	//cout<<"!\n";
	for(auto x:m2)
	{
		//cout<<x.first.first<<" "<<x.first.second.first<<" "<<x.first.second.second<<" "<<x.second<<"\n";
	}
	int ansnxtst[cost][ni],ansout[cost][ni],point=0;
	for(auto x:m2)
	{
		set<int>s,o;
		a1=x.first.first;
		a2=x.first.second.first;
		a3=x.first.second.second;
		for(int i=0;i<ni;i++)
		{
			if(a1>=0)
			{
				if(nextst[a1][i]>=0)
					s.insert(nextst[a1][i]);
				if(outs[a1][i]>=0)o.insert(outs[a1][i]);
			}
			if(a2>=0)
			{
				if(nextst[a2][i]>=0)
					s.insert(nextst[a2][i]);
				if(outs[a2][i]>=0)o.insert(outs[a2][i]);
			}
			if(a3>=0)
			{
				if(nextst[a3][i]>=0)
					s.insert(nextst[a3][i]);
				if(outs[a3][i]>=0)o.insert(outs[a3][i]);
			}
			int uy=s.size();
			if(uy==3)ansnxtst[point][i]=1;
			else if(uy==2)
			{
				vector<int>l;
				for(auto nin:s){l.push_back(nin);}
				ansnxtst[point][i]=m2[{-1,{l[0],l[1]}}];
			}
			else
			{
				vector<int>l;
				for(auto nin:s){l.push_back(nin);}
				ansnxtst[point][i]=m2[{-1,{-1,l[0]}}];
			}
			vector<int>oi;
			for(auto nin:o){oi.push_back(nin);}
			ansout[point][i]=oi[0];
		}
		point++;
	}
    return 0;
    

}
