#ifndef K_MEANS_HPP_INCLUDED
#define K_MEANS_HPP_INCLUDED

#include <vector>
#include <hamkoutil.hpp>
class ClusterizerData {
    vd m_data;
    int m_cluster_id;
public:
    ClusterizerData(size_t data_size, int cluster_id);
    ClusterizerData(size_t data_size);
    virtual ~ClusterizerData(void);
    size_t getSize(void);
    void setData(int index, double val);
    void setID(int cluster_id);
    double getData(int i);
    int getID(void);
    double calcDistance(ClusterizerData* data);
    void print(void);
};

class Clusterizer {
protected:
    vector<ClusterizerData*> m_train_data; // owes Clusterizer
public:
    Clusterizer(void);
    virtual ~Clusterizer(void);
    void addData(ClusterizerData* data);
    size_t getTrainDataSize(void);

    virtual void train(int cluster_num) = 0;
    virtual void clusterize(ClusterizerData* data) = 0;
    void printTrainData(void);
};

// ClusterizerData*の管理責任はKMeansClusterizerにある
class KMeansClusterizer : public Clusterizer {
    vector<ClusterizerData*> m_center; // owes KMeansClusterizer and initCenter
    void deleteCenter(void);
    void initCenter(int cluster_num);
    int reviseCenter(void); // 0: done, 1: the others
    int getMinimumClusterID(ClusterizerData* train_data_ti);
public:
    KMeansClusterizer(void);
    ~KMeansClusterizer(void);
    void train(int cluster_num);
    size_t getCenterNum(void);
    void clusterize(ClusterizerData* data);
    void printCenter(void);
};

#endif
