/**
 *
 * This file contains the declaration of the Path class and all associated member functions and attributes.
 * The Path class is used to hold dimensions of the mower and lawn, and to generate a set of instructions based on these values.
 * The resulting double ended queue is iterated through and provides the logic for the lawn mower
 *
 */

#ifndef PATH_H
#define PATH_H

#include <string>
#include <deque>
#include "Instruction.h"

class Path {
    public:
        Path(double length, double width, double carDiameter, double bladeDiameter);
        ~Path();
        double getLength();
        double getWidth();
        double getCarDiameter();
        double getBladeDiameter();
        std::deque<Instruction> getInstructions();
        int setDimensions(double length, double width);
		
    protected:
		
	private:
        double m_length;
        double m_width;
        double m_carDiameter;
        double m_bladeDiameter;
        std::deque<Instruction> m_instructions;

        int generatePath();
        double calculateRemainder(double numer, double denom);
        void addAFew(std::string firstMove, double firstDis,std:: string secMove="", double secDis=0, std::string thirdMove="", double thirdDis=0, std::string fourthMove = "", double fourthDis=0);
        void addConditionally(double condition, std::string move, double firstDis, double secDis);
};

#endif // PATH_H
