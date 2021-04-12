/**
 * This file contains the implementation of the Path class and all associated member functions that are included in the Path.h file.
 * The Path class is used to hold dimensions of the mower and lawn, and to generate a set of instructions based on these values.
 * 
 */

#include "Path.h"
#include <iostream>
#include <cmath>
#include <string>

/**
 * Constructor that takes in the dimensions of the lawn, as well as dimensions of the motor/blade
 * It will later use the car/blade diameters to calculate path and take car/blade dimensions into account
 *
 * @param length: length of the lawn, user determined by pressing buttons
 * @param width: width of the lawn, user determined
 * @param carDiameter: diameter of the lawn mower, used to understand how car size will impact mowing
 * @param bladeDiameter: diameter of the mowers blade underneath
 *
 */
Path::Path(double length, double width, double carDiameter, double bladeDiameter) {
    m_length = length;
    m_width = width;
    m_carDiameter = carDiameter;
    m_bladeDiameter = bladeDiameter;

    generatePath();
}

Path::~Path() {

}

/**
 * Getter function that returns the lawn length
 */
double Path::getLength() {
    return m_length;
}

/**
 * Getter function that returns the lawn width
 */
double Path::getWidth() {
    return m_width;
}

/**
 * Getter function that returns the car diameter
 */
double Path::getCarDiameter() {
    return m_carDiameter;
}

/**
 * Getter function that returns the blade diameter
 */
double Path::getBladeDiameter() {
    return m_bladeDiameter;
}

/**
 * Getter function that returns the instructions double ended queue
 */
std::deque<Instruction> Path::getInstructions() {
    return m_instructions;
}

/**
 * Setter function that sets dimensions to the new ones passed and recalculates the path
 */
int Path::setDimensions(double length, double width) {
    m_length = length;
    m_width = width;
    generatePath();

    return 0;
}

/**
 * Function that generates the path as per l/w and car/blade diameter
 * Is effectively the logic of the mower
 * @return 0: working correctly!
 * @return !0: returned by another function this function calls, experiencing an error somewhere
 */
int Path::generatePath() {
    // clear previous instructions before generating new path
    m_instructions.clear();

    double firstMoveDistance = 0;
    double secondMoveDistance = 0;

    std::cout << "m_length: " << m_length << std::endl;
    std::cout << "m_width: " << m_width << std::endl;
    std::cout << "m_carDiameter: " << m_carDiameter << std::endl;
    std::cout << "m_bladeDiameter: " << m_bladeDiameter << std::endl;
    std::cout << "\nBegin Path: \n" << std::endl;
    
    // Mower will always start with short side on left (could be input as length or width, depending on user)
    if (m_length > m_width) {
        firstMoveDistance = m_width - m_carDiameter;
        secondMoveDistance = m_length - m_carDiameter * 2;
    } else {
        firstMoveDistance = m_length - m_carDiameter;
        secondMoveDistance = m_width - m_carDiameter * 2;
    }

    // move forward ___ seconds based on firstMoveDistance
    std::cout << "MF" << firstMoveDistance << std::endl;

    // turn right 90 degrees
    std::cout << "TR90" << std::endl;

    // move forward ___ seconds based on secondMoveDistance
    std::cout << "MF" << secondMoveDistance << std::endl;

    addAFew("MF", firstMoveDistance, "TR", 90, "MF", secondMoveDistance);


    int loopCount = 0;
    double remainder = 0.0;

    // Calculate number of strips we need to cut (lengthwise)
    // The remainder is the width of the final strip that we need to cut
    if (m_length > m_width) {
        loopCount = std::ceil((m_width - m_carDiameter) / m_bladeDiameter);
        remainder = calculateRemainder(m_width - m_carDiameter, m_bladeDiameter);
    } else {
        loopCount = std::ceil((m_length - m_carDiameter) / m_bladeDiameter);
        remainder = calculateRemainder(m_length - m_carDiameter, m_bladeDiameter);
    }

    double stripWidth = m_bladeDiameter; // amount to move forward between strips (moving down)
    double stripLength = secondMoveDistance - m_bladeDiameter; // amount to move forward for each strip

    // Generate instructions for each strip of grass that we need to cut
    for (int i = 0; i < loopCount - 1; i++) {

        // Mower will turn left/right, depending on if we are cutting an even or odd numbered strip
        if (i % 2 == 0) {
            std::cout << "TR90" << std::endl;
            std::cout << "MB" << m_carDiameter - stripWidth << std::endl;
            std::cout << "TR100" << std::endl;

            addAFew("TR", 90, "MB", m_carDiameter - stripWidth, "TR", 100);

        } else {
            std::cout << "TL90" << std::endl;
            std::cout << "MB" << m_carDiameter - stripWidth << std::endl;
            std::cout << "TL120" << std::endl;

            addAFew("TL", 90, "MB", m_carDiameter - stripWidth, "TL", 120);
        }
        
        std::cout << "MF" << stripLength << std::endl;
        addAFew("MF", stripLength);
    }

    // Generate instructions for cutting final strip (usually this will be smaller than m_bladeDiameter, so we use remainder value)
    if (loopCount % 2 == 0) {
        std::cout << "TR90" << std::endl;

        addAFew("TR", 90);
        addConditionally(remainder, "MB", stripWidth, remainder);



        addAFew("TR", 100, "MF", stripLength, "TL", 180, "MF", secondMoveDistance);

        addAFew("TR", 90, "MB", m_carDiameter * 2);
    } else {
        std::cout << "TL110" << std::endl;

        addAFew("TL", 110);
        addConditionally(remainder, "MB", stripWidth, remainder);

        std::cout << "TL120" << std::endl;
        std::cout << "MF" << secondMoveDistance << std::endl;
        std::cout << "TR100" << std::endl;
        std::cout << "MB" << m_carDiameter << std::endl;

        addAFew("TL", 120, "MF", secondMoveDistance);

        addAFew("TR", 100, "MB", m_carDiameter);
    }

    return 0;
}

/**
 * Function used to calculate the remainder of a lawn we weren't able to cut because of the lawn mower or blade size
 * Used to understand how much is left so we can take that into account in path creation/movement
 */
double Path::calculateRemainder(double numer, double denom) {
    while (numer >= denom) {
        numer -= denom;
    }

    return numer;
}


/**
 * Helper function used with the generatePath function to generate the instructions
 * @param firstMove: is a turn left/right, or move forward/backward
 * @param firstDis: the distance the move will cover, these two will always be passed
 * @param secMove: is a turn left/right, or move forward/backward
 * @param secDis: the distance the move will cover, these two will always be passed as well
 * Continues on for third/fourth move/dis
 * Will effectively push them onto the deck
 */
void Path::addAFew(std::string firstMove, double firstDis, std::string secMove, double secDis, std::string thirdMove, double thirdDis, std::string fourthMove , double fourthDis){
    m_instructions.push_back(Instruction{firstMove, firstDis});

    if(secMove != ""){
        m_instructions.push_back(Instruction{secMove, secDis});
    }

    if(thirdMove != ""){
        m_instructions.push_back(Instruction{thirdMove, thirdDis});
    }

    if(fourthMove != ""){
        m_instructions.push_back(Instruction{fourthMove, fourthDis});
    }
}


/**
 * Helper function used with the generatePath function to generate the instructions
 * Adds instructions based on a passed condition
 * @param condition: remainder passed, checks if its 0 or not
 * @param move: move forward/backward
 * @param firstDis: stripwidth, buffer caused by blade diameter
 * @param secondDis: the remainder
 */
void Path:: addConditionally(double condition, std::string move, double firstDis, double secDis){
    if (condition == 0) {
        std::cout << move << firstDis << std::endl;
        m_instructions.push_back(Instruction{move, firstDis});
    } else {
        std::cout << move << secDis << std::endl;
        m_instructions.push_back(Instruction{move, secDis});
    }
}
