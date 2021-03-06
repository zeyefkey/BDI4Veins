package jasonveins.omnet.agent.utilityFunctions;

import jasonveins.omnet.environment.dijkstra.Graph;

import java.util.*;

public class CGaussianUtility implements IUtilityFunction {

    private int committeeTolerance;
    private int speedTolerance;
    private List <Integer> preferredRoute;
    private int preferredSpeed;
    private int hammingBuffer;
    private double factor;

    public CGaussianUtility(){
        this(300, 7);
    }

    public CGaussianUtility(int speedTolerance, int committeeTolerance){
        this.speedTolerance = speedTolerance;
        this.committeeTolerance = committeeTolerance;
        factor = 0.05;
    }

    @Override
    public double computeUtilitySpeed(double val) {
        double exponent = -1*(Math.pow((val - preferredSpeed),2) / speedTolerance);
        return Math.pow(Math.E, exponent);
    }

    @Override
    public double computeUtilityRoute(List<Integer> route, Graph routeGraph) {
        int inverseHammming = getHammingDistance(route, routeGraph);
        int maxHamming = routeGraph.getVertexes().size();
        double exponent = -1*(Math.pow((inverseHammming - maxHamming),2) / committeeTolerance);
        return Math.pow(Math.E, exponent);
    }

    public double computeUtilityRouteBitVector(List<Integer> route, Graph routeGraph){

        Set<Integer> vertexes = routeGraph.getVertexesMap().keySet();
        ArrayList<Integer> preferredVector = new ArrayList<>(Collections.nCopies(vertexes.size(), 0));
        Iterator<Integer> it = vertexes.iterator();
        int position = 0;
        while(it.hasNext()){
            int node = it.next();
            if(preferredRoute.contains(node))
                preferredVector.set(position, 1);
            position++;
        }
        int hammingDistance = preferredVector.size();
        for(int i = 0; i < route.size(); i++){
            if(!route.get(i).equals(preferredVector.get(i))) hammingDistance--;
        }
        hammingBuffer = preferredVector.size() - hammingDistance;


        int maxHamming = routeGraph.getVertexes().size();
        double exponent = -1*(Math.pow((hammingDistance - maxHamming),2) / committeeTolerance);
        return Math.pow(Math.E, exponent);
    }

    @Override
    public int getHammingDistance(List<Integer> evalArray, Graph route) {
        Set<Integer> vertexes = route.getVertexesMap().keySet();
        ArrayList<Integer> preferredVector = new ArrayList<>(Collections.nCopies(vertexes.size(), 0));
        ArrayList<Integer> evalBit = new ArrayList<>(Collections.nCopies(vertexes.size(), 0));
        Iterator<Integer> it = vertexes.iterator();
        int position = 0;
        while(it.hasNext()){
            int node = it.next();
            if(preferredRoute.contains(node))
                preferredVector.set(position, 1);
            position++;
        }
        it = vertexes.iterator();
        position = 0;
        while(it.hasNext()){
            int node = it.next();
            if(evalArray.contains(node))
                evalBit.set(position, 1);
            position++;
        }
        /*
        [1,1,0,0,1,1,1]
        [2,4,7,8]
        [0,0,0,0,0,0,0]
         */

        //get hamming distance, inverted in order to work with preexisting plans
        int hammingDistance = preferredVector.size();
        for(int i = 0; i < evalArray.size(); i++){
            if(!evalBit.get(i).equals(preferredVector.get(i))) hammingDistance--;
        }
        hammingBuffer = hammingDistance;
        return hammingDistance;
    }


    public int getHammingDistance() {
        return hammingBuffer;
    }

    @Override
    public void setPreferredSpeed(int speed) {
        preferredSpeed = speed;
    }

    @Override
    public void setPreferredRoute(List<Integer> route) {
        preferredRoute = route;
    }

    @Override
    public int getPreferredSpeed() {
        return preferredSpeed;
    }

    @Override
    public List<Integer> getPreferredRoute() {
        return preferredRoute;
    }
}
