//
// Created by Adhyyan Sekhsaria on 11/20/2019.
// Displays a bar graph given the information
// See GPS Test Suite for usage examples
//

#ifndef HAMSTER_BARGRAPH_H
#define HAMSTER_BARGRAPH_H

#include<iostream>
#include<algorithm>
#include<string>

class BarGraph{

private:
    size_t graphHeight;

    size_t labelRows;
    size_t numCols;

    uint32_t* height;
    std::string* labels; //2d array [numCols][labelRows]
    std::string yaxisLabel, xaxisLabel, title;

    double scalingValue = 0;

public:

    /**
     * @param numCols Number of entries
     */
    void setNumCols(size_t numCols);

    /**
     * @param labelRows Number of rows to print below the X-axis per column stacked vertically
     */
    void setLabelRows(size_t labelRows);

    /**
     * @param height An array of size numCols containing the y-axis values
     */
    void setHeights(uint32_t* height);

    /**
     * This class supports multiple labels for each columns stack vertically
     * There is a 4 char limit for each string/
     * @param labels 2d array storing the values.
     *        NOTE: Even though its a 2d array, the param is a single pointer
     *        to a flattened array of the form labels[numCols][labelRows]
     */
    void setLabels(std::string* labels);

    /**
     * Sets axis titles which is displayed on the bar graph
     */
    void setAxis(std::string title, std::string yaxis, std::string xaxis);

    /**
     *
     * @param gheight Number of rows to print in the bar graph.
     * Height values will be scaled by the scalingFactor and graphHeight
     */
    void setGraphHeight(size_t gheight);

    /**
     * @param scalingValue Max value which corresponds to the top of the graph.
     *        if set to 0, then will do auto scaling by choosing the max from the heights
     */
    void setScalingValue(double scalingValue);

    /**
     * Prints the bar graph.
     * Recommended to clear the screen before printng to avoid clutter.
     * printf(VT100_CLEAR_SCREEN VT100_CURSOR_HOME); to clear and reset cursor
     */
    void print();

private:

    /**
     * Centers text in the inp std::string, with the same amount of whitespace on both sides and prints
     * If input is longer than outputLen, then prints the truncated string
     */
    static void printCenteredText(const std::string &inp, int outputLen);

    void printHeader();

    void printGraph();

    void printBottomStuff();

};


#endif //HAMSTER_BARGRAPH_H
