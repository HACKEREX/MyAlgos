#include<cstdio>
#include<vector>
#include<limits.h>
#include<algorithm>

/* Coded Using Wavelet Trees

Author: Avinash Kumar
email : ak007avisonu@gmail.com
	
	Function small: Finds SMallest kth number in range i,j inclusive
		Signature : int (left_index,right_index,kth_smallest,root_node)

	Function ksmall : Finds kth SMallest number within given last index inclusive
		Signature : int (last_index,kth_smallest,root_node)

	Function print : Prints A Node at depth k from given parent Node.
		Signature int (Node * pointer_to_node,child_at_depth_k)

	Function rank_of: Finds No of Times a givn number is present in range.
		Signature :int (given_no,given_last_index,root_node)

	Function head : Returns the Root of Wavelet Tree
		Signature node* ()

		It's A Solution To Problem MKTHNUM from Spoj
    http://www.spoj.com/problems/MKTHNUM/
*/ 
using namespace std;

struct node{

	int min_element,max_element;
	bool leaf;
	vector<int> mainarray;
	vector<int> sizearray;

	node *left,*right;

	node():leaf(false),left(NULL),right(NULL)
	{}
};

class wavelet{

	int N;
	int *base_array;
	node *tr;

	void build_tree(node * p)
	{
		node * l = new node;
		node * r = new node;

		

		vector<int>::iterator it=p->mainarray.begin();

		int mid_element=(p->max_element+p->min_element)>>1;
		int lsize=0;
		int rsize=0;
		int index=0;
		int lmin=INT_MAX,rmin=INT_MAX,lmax=INT_MIN,rmax=INT_MIN;
		for(it;it!=p->mainarray.end();it++)
		{
			if(*(it)>mid_element)
			{
				p->sizearray[index++]=lsize;
				r->mainarray.push_back(*(it));
				rmax=max(rmax,*(it));
				rmin=min(rmin,*(it));
			}else
			{
				p->sizearray[index++]=lsize++;
				l->mainarray.push_back(*(it));
				lmax=max(lmax,*(it));
				lmin=min(lmin,*(it));
			}
		}

		l->min_element=lmin;
		l->max_element=lmax;
		r->min_element=rmin;
		r->max_element=rmax;

		if(lmax==lmin)
			l->leaf=true;
		else
			l->leaf=false;

		if(rmax==rmin)
			r->leaf=true;
		else
			r->leaf=false;

		l->sizearray.assign(l->mainarray.size(),0);
		r->sizearray.assign(r->mainarray.size(),0);

		p->left=l;
		p->right=r;

		if(l->leaf==false)
			build_tree(p->left);
		if(r->leaf==false)
			build_tree(p->right);

	}

public:
	wavelet(int tempar[],int n)
	{
		N=n;
		base_array=new int[n];
		tr = new node;

		tr->mainarray.assign(n,0);
		int mn=INT_MAX,mx=INT_MIN;
		for(int i=0;i<n;i++)
		{
			base_array[i]=tempar[i];
			tr->mainarray[i]=tempar[i];

			mn=min(tr->mainarray[i],mn);
			mx=max(tr->mainarray[i],mx);
		}

		tr->max_element=mx;
		tr->min_element=mn;
		tr->sizearray.assign(n,0);

		if(tr->min_element==tr->max_element)
			tr->leaf=true;
		else tr->leaf=false;

		if(!tr->leaf)
			build_tree(tr);
	}

	node *head()
	{return tr;}

	void print(node *p,int level=0)
	{
		if(p==NULL)
			return;
		if(level==0)
		{
			printf("Main Array\n");
			for(int i=0;i<p->mainarray.size();i++)
				printf("%d ",p->mainarray[i]);
			printf("\n");
			printf(" Min element %d\n", p->min_element);
			printf(" Max element %d\n", p->max_element);
			
			printf("LeftIndexing array\n");
			for(int i=0;i<p->sizearray.size();i++)
				printf("%d ",p->sizearray[i]);
			printf("\n\n\n");
			return;
		}
		print(p->left,level-1);
		print(p->right,level-1);
	}

	int rank_of(int elem,int range,node * p=NULL)
	{
		if(range<0)
			return 0;
		range=min(range,(int)p->mainarray.size()-1);

		if(p->leaf==true )
			return (elem==(int)p->mainarray[range])?range+1:0;
		int nr=range;

		if(p!=NULL)
		{
			int mid_element=(p->max_element+p->min_element)>>1;
			int range_elem=p->mainarray[range];
			if(elem>mid_element)
			{
				if(range_elem>mid_element)
					{
						nr=range-(p->sizearray[range]);
						return	rank_of(elem,nr,p->right);
					}
				else
					{
						nr=range-(p->sizearray[range])-1;
						return rank_of(elem,nr,p->right);
					}
				
			}
			else
			{
				if(range_elem>mid_element)
				{
					nr=(p->sizearray[range])-1;
					return rank_of(elem,nr,p->left);
				}else
				{
					nr=(p->sizearray[range]);
					return rank_of(elem,nr,p->left);
				}
			}
		}
		return 0;
	}

	int ksmall(int range,int k,node * p)
	{
		if(p->leaf && k>0 && k<=p->mainarray.size())
			return p->mainarray[k-1];
		
		int elem=p->mainarray[range];
		int posl=p->sizearray[range];
		int mid_element=(p->max_element+p->min_element)>>1;
		if(elem<=mid_element)
			posl++;
		if(posl>=k)
			return ksmall(posl-1,k,p->left);
		else
		{
			posl=range-p->sizearray[range];
			int offset=p->sizearray[range];

			if(mid_element>=p->mainarray[range])
				{posl--;offset++;}			
			return ksmall(posl,k-offset,p->right);
		}
	}

	int small(int i,int j,int k,node *p)
	{

		if(p->leaf && k<=p->mainarray.size())
			return p->mainarray[k-1];

		int ei=p->mainarray[i];
		int ej=p->mainarray[j];
		int mid_element=(p->max_element+p->min_element)>>1;

		int pi=p->sizearray[i];
		int pj=p->sizearray[j]-1;
		if(ej<=mid_element)
			pj++;
		int loffset=(pj-pi+1);

		if(loffset>=k)
			return small(pi,pj,k,p->left);
		else
		{
			int ni=i-p->sizearray[i];
			int nj=j-p->sizearray[j]-1;
			if(ej>mid_element)
				nj++;


			if(ni==0)
				return ksmall(nj,k-loffset,p->right);
			else
				return small(ni,nj,k-loffset,p->right);

		}


	}

		
};

int main()
{
	
	int n,m;
	scanf("%d %d",&n,&m);

	int ar[n];
	for(int i=0;i<n;i++)
		scanf("%d",&ar[i]);
	wavelet wv(ar,n);

	while(m--)
	{
		int i,j,k;
		scanf("%d %d %d",&i,&j,&k);
		i--;j--;
		printf("%d\n",wv.small(i,j,k,wv.head()));
	}
	
}
