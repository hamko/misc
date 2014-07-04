#include <hamkoutil.hpp>
#include "k-means.hpp"

/****************/
/* Cluster Data */
/****************/
ClusterizerData::ClusterizerData(size_t data_size, int cluster_id)
{
    m_data.resize(data_size, 0);
    setID(cluster_id);
}
ClusterizerData::ClusterizerData(size_t data_size)
{
    m_data.resize(data_size, 0);
}
void ClusterizerData::setData(int index, double val)
{
    m_data.at(index) = val;
}
void ClusterizerData::setID(int cluster_id)
{
    m_cluster_id = cluster_id;
}
ClusterizerData::~ClusterizerData(void)
{
}
size_t ClusterizerData::getSize(void)
{
    return m_data.sz;
}
double ClusterizerData::getData(int i)
{
    return m_data.at(i);
}
int ClusterizerData::getID(void)
{
    return m_cluster_id;
}
double ClusterizerData::calcDistance(ClusterizerData* data)
{
    assert(data->getSize() == this->getSize());

    double sum = 0;
    for (size_t cdi = 0; cdi < getSize(); cdi++) 
        sum += square(this->getData(cdi) - data->getData(cdi));
//    cout << sum << endl;
    return sqrt(sum);
}
void ClusterizerData::print(void)
{
    cout << getID() << ": ";
    for (size_t i = 0; i < m_data.sz; i++) {
        cout << getData(i) << " ";
    }
    cout << endl;
}

/****************/
/* Clusterizer  */
/****************/

Clusterizer::Clusterizer(void)
{
}

size_t Clusterizer::getTrainDataSize(void)
{
    return m_train_data.sz;
}

Clusterizer::~Clusterizer(void)
{
    for (size_t i = 0; i < getTrainDataSize(); i++)
        delete m_train_data.at(i);
}

void Clusterizer::addData(ClusterizerData* data)
{
    assert(m_train_data.empty() || m_train_data.at(0)->getSize() == data->getSize());
    m_train_data.pb(data); 
}

void Clusterizer::printTrainData(void)
{
    for (size_t ti = 0; ti < getTrainDataSize(); ti++) 
        m_train_data.at(ti)->print();
}

/****************/
/*    KMeans    */
/****************/
KMeansClusterizer::KMeansClusterizer(void)
{
}

KMeansClusterizer::~KMeansClusterizer(void)
{
    deleteCenter();
}

void KMeansClusterizer::deleteCenter(void)
{
    if (m_center.sz) 
        for (size_t ci = 0; ci < m_center.sz; ci++) 
            delete m_center.at(ci);
}

size_t KMeansClusterizer::getCenterNum(void)
{
    return m_center.sz;
}

// ref: http://tech.nitoyon.com/ja/blog/2009/04/09/kmeans-visualise/
void KMeansClusterizer::train(int cluster_num)
{
    initCenter(cluster_num);
    while (reviseCenter());
}

void KMeansClusterizer::initCenter(int cluster_num)
{
    // 0. Admin Center Instance
    deleteCenter();
    m_center.resize(cluster_num, NULL);
    size_t data_size = m_train_data.at(0)->getSize();
    for (size_t ci = 0; ci < getCenterNum(); ci++) 
        m_center.at(ci) = new ClusterizerData(data_size, 0);

    // 1. Random
    for (size_t ti = 0; ti < getTrainDataSize(); ti++) {
        auto train_data_ti = m_train_data.at(ti);
        train_data_ti->setID(rand() % getCenterNum());
    }
}

int KMeansClusterizer::getMinimumClusterID(ClusterizerData* train_data_ti)
{
    vd dist_v(getCenterNum());
    for (size_t ci = 0; ci < dist_v.sz; ci++) {
        auto center_ci = m_center.at(ci);
        dist_v.at(ci) = train_data_ti->calcDistance(center_ci);
    }
    return (int)(min_element(all(dist_v)) - begin(dist_v));
}

void KMeansClusterizer::printCenter(void)
{
    for (size_t ci = 0; ci < getCenterNum(); ci++) 
        m_center.at(ci)->print();
}

int KMeansClusterizer::reviseCenter(void) {
    // pre-4. Store Initial Cluster ID
    vi pre_id_v(getTrainDataSize());
    for (size_t ti = 0; ti < getTrainDataSize(); ti++) 
        pre_id_v.at(ti) = m_train_data.at(ti)->getID();

    // 2. Calc Center 
    vi vn(getCenterNum(), 0);
    for (size_t ti = 0; ti < getTrainDataSize(); ti++) {
        auto train_data_ti = m_train_data.at(ti);
        auto id_ti = train_data_ti->getID();
        vn.at(id_ti)++;
        for (size_t cdi = 0; cdi < (m_train_data.at(ti))->getSize(); cdi++) {
            auto data_ti_cdi = train_data_ti->getData(cdi), cluster_ti_cdi = m_center.at(id_ti)->getData(cdi);
            m_center.at(id_ti)->setData(cdi, cluster_ti_cdi + data_ti_cdi);
        }
    }
    for (size_t ci = 0; ci < getCenterNum(); ci++) {
        if (!vn.at(ci)) {
            cerr << "Cluster Size = 0, aborted" << endl;
            exit(1);
        }
        auto center_ci = m_center.at(ci);
        for (size_t cdi = 0; cdi < center_ci->getSize(); cdi++) {
            center_ci->setData(cdi, center_ci->getData(cdi) / vn.at(ci));
        }
    }

    // 3. Change ID
    for (size_t ti = 0; ti < getTrainDataSize(); ti++) 
        m_train_data.at(ti)->setID(getMinimumClusterID(m_train_data.at(ti)));

    // 4. Check if Revision has Occurred
    for (size_t ti = 0; ti < getTrainDataSize(); ti++) 
        if (pre_id_v.at(ti) != m_train_data.at(ti)->getID())
            return 1;

    return 0;
}

// ClusterizerData.m_cluster_idに書き込む
void KMeansClusterizer::clusterize(ClusterizerData* data) 
{
}
