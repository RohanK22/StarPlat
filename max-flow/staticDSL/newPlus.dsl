function do_max_flow (Graph g, node source, node sink, propEdge<int> residual_capacity) {

    propNode<int> label;
    g.attachNodeProperty (label = 0) ;
    source.label = g.num_nodes();
    
    propNode<int> excess;
    g.attachNodeProperty (excess = 0) ;

    propNode<int> curr_edge;
    g.attachNodeProperty (curr_edge = 0) ;

    forall (v in g.neighbors(source)) {
        edge forward_edge = g.get_edge(source, v) ;
        edge backward_edge = g.get_edge(v, source) ;

        int d = forward_edge.residual_capacity;

        v.excess = d;

        int temp;
        temp = forward_edge.residual_capacity - d ;
        forward_edge.residual_capacity = temp;

        temp = backward_edge.residual_capacity + d ;
        backward_edge.residual_capacity = temp;
    }
    
    bool flag;

    do{
        flag = False;
        forall (u in g.nodes().filter(u!=source && u!=sink && u.excess>0)) {
            flag = True;
            do{
                forall(v in g.neighbors(u)) {
                    edge current_edge = g.get_edge (u,v) ;
                    if (u.excess > 0 && current_edge.residual_capacity > 0) {
                        edge forward_edge = g.get_edge (u, v) ;
                        edge backward_edge = g.get_edge (v, u) ;
                        int d = min (u.excess, forward_edge.residual_capacity) ;

                        int temp ;

                        u.excess -= d ;
                        v.excess += d ;
                        backward_edge.residual_capacity += d ;
                        forward_edge.residual_capacity -= d ;
                    }
                }

                if (u.excess > 0) {
                    int new_label = INT_MAX ;
                    forall(v in g.neighbors (u)) {
                        edge residual = g.get_edge (u, v) ;
                        if (residual.residual_capacity > 0) {
                            if (new_label < v) {
                                new_label = v ; 
                            }
                        }
                    }

                    if (new_label < INT_MAX) {
                        u.label = new_label+1 ;
                    }
                }
            }while(u.excess>0);
        }
    }while(flag);
}
