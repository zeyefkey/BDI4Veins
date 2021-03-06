package jasonveins.omnet.voting.rule;

import jasonveins.omnet.voting.CUtilityPair;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class CCopeland implements IRule {

    @Override
    public int getResult(List<List<Integer>> votes, List<Integer> candidates) {
        scoreVector.clear();
        boolean ties = false;
        int winnerIndex = -1;
        int highestScore = -1;
        int voteVectorLength = votes.get(0).size();
        for(int i = 0; i < voteVectorLength; i++){
            int score = 0;
            for(int j = 0; j < votes.size(); j++){
                score += votes.get(j).get(i);
            }
            if(score == highestScore){
                ties = true;
                m_tiedIndexes.add(i);
            }else if(score > highestScore){
                highestScore = score;
                winnerIndex = i;
                ties = false;
                m_tiedIndexes.clear();
            }
            scoreVector.add(score);
        }
        if(ties) return -1;
        return winnerIndex;
        /*scoreVector.clear();
        List<Integer> pairWiseWins = new ArrayList<>(Collections.nCopies(candidates.size(), 0));
        int position = 0;
        for(int i = 0; i < candidates.size() - 1; i++) {
            for (int j = i + 1; j < candidates.size(); j++) {
                int scoreLeft = 0;
                int scoreRight = 0;
                for (int z = 0; z < votes.size(); z++) {
                    scoreLeft += votes.get(z).get(position * 2);
                    scoreRight += votes.get(z).get(position * 2 + 1);
                }
                if (scoreLeft > scoreRight) {
                    pairWiseWins.set(i, pairWiseWins.get(i) + 1);
                } else {
                    pairWiseWins.set(j, pairWiseWins.get(j) + 1);
                }
                position++;
            }
        }
        scoreVector.addAll(pairWiseWins);
        return pairWiseWins.indexOf(Collections.max(pairWiseWins));*/
    }

    /**
     * Generates a vote vector based on the received utilities
     * Vote vector is sorted as follows:
     * Given candidate list [c1,c2,c3]
     * Vote index is organized as
     * c1.c2
     * c1.c3
     * c2.c3
     * Value of index is 0,1 if preference is given to the candidate in the right side or 1,0 otherwise
     * i.e: for c1>c3>c2 the vote vector would be [1,0,1,0,0,1]
     *
     * @param p_utilities A list containing mapping of the index to its utility, ordered bottom to top
     * @return An integer list with the votes according to the voting rule
     */
    @Override
    public List<Integer> getVote(List<CUtilityPair> p_utilities) {
        ArrayList<Integer> votes = new ArrayList<>(Collections.nCopies(p_utilities.size(), 0));
        /*for(int i = 0; i < p_utilities.size() - 1; i++){
            for(int j = i + 1; j < p_utilities.size(); j++){
                if(p_utilities.get(i).getUtility() > p_utilities.get(j).getUtility()){
                    votes.add(1); votes.add(0);
                }
                else{
                    votes.add(0);  votes.add(1);
                }
            }
        }*/
        for(int i = 0; i < p_utilities.size() - 1; i++){
            for(int j = i + 1; j < p_utilities.size(); j++){
                if(p_utilities.get(i).getUtility() > p_utilities.get(j).getUtility())
                    votes.set(i, votes.get(i) + 1);
                else
                    votes.set(j, votes.get(j) + 1);
            }
        }
        return votes;
    }

    /**
     * Generates a vote intended to brake a tie.
     *
     * @param p_utilities A list containing mapping of the index to its utility, ordered bottom to top
     * @return A modified integer list with the votes according to the voting rule
     */
    @Override
    public List<Integer> getTieBreakerVote(List<CUtilityPair> p_utilities, List<Integer> p_ties) {
        return getVote(p_utilities);
    }

    @Override
    public List<Integer> getTiedIndexes(){
        return m_tiedIndexes;
    }

    @Override
    public List<Integer> reduceCandidates(List<List<Integer>> votes, List<Integer> candidates){
        if(candidates.size() <= 2)
            return null;
        else{
            int newSize = candidates.size()/2;
            ArrayList<Integer> redCandidates = new ArrayList<>(newSize);
            this.getResult(votes, candidates);
            for(int i = 0; i < newSize; i++){
                int highestIndex = scoreVector.indexOf(Collections.max(scoreVector));
                redCandidates.add(candidates.get(highestIndex));
                scoreVector.set(highestIndex, -1);
            }
            return redCandidates;
        }
    }

    @Override
    public int getExpectedVoteSize(int size){
        return size;
        /*int l_size = 0;
        for(int i = size - 1; i > 0; i--){
            l_size += i;
        }
        return l_size*2;*/
    }

    @Override
    public List<Integer> getResultVector(List<List<Integer>> votes, List<Integer> candidates, int scoreThreshold){
        getResult(votes, candidates);
        return scoreVector;
    }
}
