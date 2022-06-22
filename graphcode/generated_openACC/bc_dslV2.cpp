#include"bc_dslV2.h"

void Compute_BC(graph& g,float* BC,std::set<int>& sourceSet)
{

  #pragma acc data copyin(g)
  {
    #pragma acc data copyout( BC[0: g.num_nodes()] )
    {
      #pragma acc parallel loop
      for (int t = 0; t < g.num_nodes(); t ++) 
      {
        BC[t] = 0;
      }
    }
  }

  std::set<int>::iterator itr;
  for(itr=sourceSet.begin();itr!=sourceSet.end();itr++)
  {
    int src = *itr;
    double* sigma=new double[g.num_nodes()];
    int* bfsDist=new int[g.num_nodes()];
    float* delta=new float[g.num_nodes()];

    #pragma acc data copyin(g)
    {
      #pragma acc data copyout( delta[0: g.num_nodes()], bfsDist[0: g.num_nodes()] )
      {
        #pragma acc parallel loop
        for (int t = 0; t < g.num_nodes(); t ++) 
        {
          delta[t] = 0;
          bfsDist[t] = -1;
        }
      }
    }


    #pragma acc data copyin(g)
    {
      #pragma acc data copyout( sigma[0: g.num_nodes()] )
      {
        #pragma acc parallel loop
        for (int t = 0; t < g.num_nodes(); t ++) 
        {
          sigma[t] = 0;
        }
      }
    }

    bfsDist[src] = 0;
    sigma[src] = 1;
    int* level = new int[g.num_nodes()];
    int dist_from_source = 0;
    int finished = 0;


    #pragma acc data copy(g)
    {
      #pragma acc data copy(level[0:g.num_nodes()])
      {
        #pragma acc parallel loop
        for(int int t=0; t<g.num_nodes(); t++)
        {
          level[t] = -1;
        }
      }
    }


    level[src] = 0;
    #pragma acc data copyin(g)
    {
      #pragma acc copyin(src, offset[0:g.num_nodes()+1], edge_array[0:g.num_edges()]) copy(delta[0:g.num_nodes()], sigma[0:g.num_nodes()], level[0:g.num_nodes()], BC[0:g.num_nodes()])
      {
        do
        {
          finished = 1;
          #pragma acc data copy(finished, dist_from_source)
          {
            #pragma acc parallel loop
            for (int v=0; v<g.num_nodes(); v++)
            {
              if(level[v] == dist_from_source)
              {
                for (int edge = g.indexofNodes[v]; edge < g.indexofNodes[v+1]; edge ++) 
                {int w = g.edgeList[edge] ;
                  if(level[w] == -1)
                  {
                    level[w] = dist_from_source + 1;
                    finished=0;
                  }
                  if(level[w] == dist_from_source+1 )
                  {
                    #pragma acc atomic update
                    sigma[w] = sigma[w] + sigma[v];
                  }
                }
              }
            }
          }
          dist_from_source++;
        }
        while(!finished);

        dist_from_source--;

        while (dist_from_source > 1)
        {
          #pragma acc data copy(dist_from_source)
          {
            #pragma acc parallel loop
            for( int v = 0; v < g.num_nodes() ; v++)
            {
              if( level[v] == dist_from_source-1 )
              {
                for (int edge = g.indexofNodes[v]; edge < g.indexofNodes[v+1]; edge ++) 
                {int w = g.edgeList[edge] ;
                  if(level[w] == dist_from_source)
                  {
                    delta[v] = delta[v] + (sigma[v] / sigma[w]) * (1 + delta[w]);
                  }
                }
                BC[v] = BC[v] + delta[v];
              }
            }
          }
          dist_from_source--;
        }
      }
    }
  }

}
