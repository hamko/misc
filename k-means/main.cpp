#include "k-means.hpp"
#include <hamkoutil.hpp>

int main(void)
{
    srand((unsigned int)time(NULL));

    KMeansClusterizer cl;

    // Train
    for (int i = 0; i < 1000; i++) {
        ClusterizerData* data = new ClusterizerData(2);
        int r = rand() % 3;
        data->setData(0, 200*r+rand()%100);
        data->setData(1, 200*r+rand()%100);
        cl.addData(data);
    }
    cl.train(3);
    cout << "#Train" << endl;
    cl.printTrainData();

    // Test
    ClusterizerData* testdata = new ClusterizerData(2);
    testdata->setData(0, 450);
    testdata->setData(1, 380);
    cl.clusterize(testdata);
    cout << "#Test" << endl;
    testdata->print();
    

    return 0;
}
