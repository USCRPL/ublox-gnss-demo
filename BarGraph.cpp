//
// Created by ASUS_PC on 11/20/2019.
//


#include "BarGraph.h"
//! order of these includes matters (BarGraph.h must come last)

void BarGraph::setNumCols(size_t numCols){
    this->numCols = numCols;
}

void BarGraph::setLabelRows(size_t labelRows){
    this->labelRows = labelRows;
}

void BarGraph::setHeights(uint32_t* height){
    this->height = height;
}

void BarGraph::setLabels(std::string* labels){
    this->labels = labels;
}

void BarGraph::setAxis(std::string title, std::string yaxis, std::string xaxis){
    this->title = title;
    this->yaxisLabel = yaxis;
    this->xaxisLabel = xaxis;
}

void BarGraph::setGraphHeight(size_t gheight){
    this->graphHeight = gheight;
}


void BarGraph::print(){
    printHeader();
    printGraph();
    printBottomStuff();
}

void BarGraph::printCenteredText(const std::string &inp, int outputLen){
    int inpLen = inp.length();

    if(inpLen > outputLen){
        printf(inp.substr(0, outputLen).c_str());
        return;
    }

    int offset = (outputLen - inpLen + 1)/2;
    std::string output(outputLen, ' ');
    for(int i = offset; i < offset + inpLen; i++) {
        output[i] = inp[i - offset];
    }
    printf(output.c_str());
}

void BarGraph::printHeader(){
    // Center and Print Title
    printCenteredText(title + " ("+yaxisLabel+ ", "+xaxisLabel+")", numCols*4);
    printf("\r\n\r\n");
}

void BarGraph::printGraph(){
    // scaling factor: if we have GRAPH_HEIGHT lines, how many dB per line?
    uint32_t maxValue = 0;
    for(size_t index = 0; index < numCols; ++index)
    {
        maxValue = std::max(maxValue, height[index]);
    }

    float scalingFactor = maxValue * 1.00f / graphHeight;
    if(scalingValue != 0) scalingFactor = scalingValue/graphHeight;
    // start at graphHeight + 1 so that we can print numbers on bars that are the full height
    for(size_t graphLine = graphHeight + 1; graphLine > 0; --graphLine)
    {
        // print left line
        printf(" |");

        float currentdBValue = graphLine * scalingFactor;
        float nextdBValue = (graphLine - 1) * scalingFactor;

        for(size_t index = 0; index < numCols; index++)
        {
            // decide what to print
            bool printBar = currentdBValue <= height[index];
            bool printNumber = !printBar && (nextdBValue <= height[index] || graphLine == 1);

            if(printBar)
            {
                printf(" ## ");
            }
            else if(printNumber)
            {
                // center-pad number
                printCenteredText(std::to_string(height[index]), 4);
            }
            else{
                printf("    ");
            }
        }
        printf("\r\n");
    }

}

void BarGraph::printBottomStuff(){
    // print bottom row
    printf(" |");
    for(size_t index = 0; index < numCols; index++)
    {
        printf("----");
    }
    printf("\r\n");

    for(size_t labelLine = 0; labelLine < labelRows; labelLine++){
        printf(" ");
        for(size_t index = 0; index < numCols; index++){
            std::string currentLabel = (*(labels + index*labelRows + labelLine)).substr(0, 4);
            printCenteredText(currentLabel, 4);
        }
        printf("\r\n");
    }
    printCenteredText(xaxisLabel, numCols * 4);

}

void BarGraph::setScalingValue(double scalingValue) {
    this->scalingValue = scalingValue;
}
