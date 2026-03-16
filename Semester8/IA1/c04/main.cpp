#include <stdio.h>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <ctime>

struct ObjectStats {
    int area = 0;
    long long sum_x = 0;
    long long sum_y = 0;
    int circ = 0;
    double xt = 0.0;
    double yt = 0.0;
    double mu20 = 0.0;
    double mu02 = 0.0;
    double mu11 = 0.0;
};

void treshold(const cv::Mat &src, cv::Mat &dst, int threshold)
{
    dst = src.clone();
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            if (src.at<uchar>(i, j) > threshold)
                dst.at<uchar>(i, j) = 255;
            else
                dst.at<uchar>(i, j) = 0;
        }
    }
}

void flood_fill(cv::Mat &src, int x, int y, uchar new_value)
{
    if (x < 0 || x >= src.cols || y < 0 || y >= src.rows)
        return; // Out of bounds

    uchar old_value = src.at<uchar>(y, x);

    if (old_value == 0)
        return; // Background, do not fill
    if (old_value == new_value)
        return; // Already filled

    src.at<uchar>(y, x) = new_value;

    flood_fill(src, x + 1, y, new_value);     // Right
    flood_fill(src, x - 1, y, new_value);     // Left
    flood_fill(src, x, y + 1, new_value);     // Down
    flood_fill(src, x, y - 1, new_value);     // Up
    flood_fill(src, x + 1, y + 1, new_value); // Down-Right
    flood_fill(src, x - 1, y + 1, new_value); // Down-Left
    flood_fill(src, x + 1, y - 1, new_value); // Up-Right
    flood_fill(src, x - 1, y - 1, new_value); // Up-Left
}

int begin_flood_fill(cv::Mat &src)
{
    uchar idx = 1;

    printf("rows: %d, cols: %d\n", src.rows, src.cols);

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            if (src.at<uchar>(i, j) == 255)
            {
                flood_fill(src, j, i, idx);
                if (idx < 255) {
                    ++idx;
                }
            }
        }
    }
    return idx;
}

struct FeatureVector {
    double F1;
    double F2;
    double xt;
    double yt;
    int area;
    int circ;
};

struct Etalon {
    std::string name;
    double F1;
    double F2;
};

std::vector<FeatureVector> extract_features(const cv::Mat& labeled, int num_objects) {
    std::vector<ObjectStats> stats(num_objects);
    
    for (int y = 0; y < labeled.rows; y++) {
        for (int x = 0; x < labeled.cols; x++) {
            uchar val = labeled.at<uchar>(y, x);
            if (val > 0 && val < num_objects) {
                stats[val].area++;
                stats[val].sum_x += x;
                stats[val].sum_y += y;
                
                // Check circumference
                bool is_boundary = false;
                if (x == 0 || labeled.at<uchar>(y, x-1) != val) is_boundary = true;
                else if (x == labeled.cols - 1 || labeled.at<uchar>(y, x+1) != val) is_boundary = true;
                else if (y == 0 || labeled.at<uchar>(y-1, x) != val) is_boundary = true;
                else if (y == labeled.rows - 1 || labeled.at<uchar>(y+1, x) != val) is_boundary = true;
                
                if (is_boundary) {
                    stats[val].circ++;
                }
            }
        }
    }
    
    for (int i = 1; i < num_objects; i++) {
        if (stats[i].area > 0) {
            stats[i].xt = (double)stats[i].sum_x / stats[i].area;
            stats[i].yt = (double)stats[i].sum_y / stats[i].area;
        }
    }
    
    for (int y = 0; y < labeled.rows; y++) {
        for (int x = 0; x < labeled.cols; x++) {
            uchar val = labeled.at<uchar>(y, x);
            if (val > 0 && val < num_objects) {
                double dx = x - stats[val].xt;
                double dy = y - stats[val].yt;
                stats[val].mu20 += dx * dx;
                stats[val].mu02 += dy * dy;
                stats[val].mu11 += dx * dy;
            }
        }
    }
    
    std::vector<FeatureVector> features;
    for (int i = 1; i < num_objects; i++) {
        if (stats[i].area > 0) {
            double mu20 = stats[i].mu20;
            double mu02 = stats[i].mu02;
            double mu11 = stats[i].mu11;
            
            double tmp1 = mu20 + mu02;
            double tmp2 = std::sqrt(4 * mu11 * mu11 + (mu20 - mu02) * (mu20 - mu02));
            double mu_max = 0.5 * (tmp1 + tmp2);
            double mu_min = 0.5 * (tmp1 - tmp2);
            
            double F1 = (double)(stats[i].circ * stats[i].circ) / (100.0 * stats[i].area);
            double F2 = (mu_max > 0.0) ? (mu_min / mu_max) : 0.0;
            
            features.push_back({F1, F2, stats[i].xt, stats[i].yt, stats[i].area, stats[i].circ});
        }
    }
    return features;
}

cv::Mat index_to_color(const cv::Mat &src)
{
    cv::Mat color_image(src.size(), CV_8UC3);
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            uchar idx = src.at<uchar>(i, j);
            if (idx == 0)
            {
                color_image.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0); // Background
            }
            else
            {
                // Assign a random color based on the index
                color_image.at<cv::Vec3b>(i, j) = cv::Vec3b((idx * 37) % 256, (idx * 59) % 256, (idx * 83) % 256);
            }
        }
    }
    return color_image;
}

void train_etalons() {
    cv::Mat image = cv::imread("../images/train.png", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        printf("Unable to read train.png.\n");
        return;
    }

    cv::Mat output;
    treshold(image, output, 128);
    int num_objects = begin_flood_fill(output);
    auto features = extract_features(output, num_objects);
    
    double sum_f1_sq = 0, sum_f2_sq = 0; int count_sq = 0;
    double sum_f1_rect = 0, sum_f2_rect = 0; int count_rect = 0;
    double sum_f1_star = 0, sum_f2_star = 0; int count_star = 0;

    for (const auto& f : features) {
        if (f.yt < 60) {
            sum_f1_sq += f.F1; sum_f2_sq += f.F2; count_sq++;
        } else if (f.yt < 110) {
            sum_f1_star += f.F1; sum_f2_star += f.F2; count_star++;
        } else {
            sum_f1_rect += f.F1; sum_f2_rect += f.F2; count_rect++;
        }
    }

    std::ofstream ofs("etalons.txt");
    if (ofs.is_open()) {
        if (count_sq > 0) ofs << "Square " << sum_f1_sq / count_sq << " " << sum_f2_sq / count_sq << "\n";
        if (count_rect > 0) ofs << "Rectangle " << sum_f1_rect / count_rect << " " << sum_f2_rect / count_rect << "\n";
        if (count_star > 0) ofs << "Star " << sum_f1_star / count_star << " " << sum_f2_star / count_star << "\n";
        ofs.close();
        printf("Saved etalons to etalons.txt\n");
    }
}

void classify_image() {
    std::vector<Etalon> etalons;
    std::ifstream ifs("etalons.txt");
    if (ifs.is_open()) {
        std::string name;
        double f1, f2;
        while (ifs >> name >> f1 >> f2) {
            etalons.push_back({name, f1, f2});
        }
        ifs.close();
    } else {
        printf("Could not find etalons.txt. Please train first.\n");
        return;
    }

    cv::Mat image = cv::imread("../images/test02.png", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        printf("Unable to read test02.png.\n");
        return;
    }

    cv::Mat output;
    treshold(image, output, 128);
    int num_objects = begin_flood_fill(output);
    auto features = extract_features(output, num_objects);
    
    cv::Mat color_image = index_to_color(output);

    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.5;
    int thickness = 1;

    for (const auto& f : features) {
        std::string best_class = "Unknown";
        double min_dist = 1e9;
        
        for (const auto& et : etalons) {
            double dist = std::sqrt(std::pow(f.F1 - et.F1, 2) + std::pow(f.F2 - et.F2, 2));
            if (dist < min_dist) {
                min_dist = dist;
                best_class = et.name;
            }
        }
        
        cv::Point textOrg(f.xt - 20, f.yt);
        cv::putText(color_image, best_class, textOrg, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness + 2);
        cv::putText(color_image, best_class, textOrg, fontFace, fontScale, cv::Scalar(255, 255, 255), thickness);
    }
    
    cv::imshow("Classification Result", color_image);
    int key;
    while (true) {
        key = cv::waitKey(0);
        if (key == 27 || key == '\r') break;
    }
}

void train_kmeans() {
    cv::Mat image = cv::imread("../images/train.png", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        printf("Unable to read train.png.\n");
        return;
    }

    cv::Mat output;
    treshold(image, output, 128);
    int num_objects = begin_flood_fill(output);
    auto features = extract_features(output, num_objects);
    
    if (features.size() < 3) {
        printf("Not enough features to run k-means with k=3\n");
        return;
    }

    int k = 3;
    std::vector<FeatureVector> centroids(k);
    
    std::srand(time(NULL));
    int c1 = std::rand() % features.size();
    int c2 = std::rand() % features.size();
    while (c2 == c1) c2 = std::rand() % features.size();
    int c3 = std::rand() % features.size();
    while (c3 == c1 || c3 == c2) c3 = std::rand() % features.size();
    
    centroids[0] = features[c1];
    centroids[1] = features[c2];
    centroids[2] = features[c3];

    bool changed = true;
    int iterations = 0;
    std::vector<int> assignments(features.size(), 0);

    while (changed && iterations < 1000) {
        changed = false;
        iterations++;
        
        std::vector<std::vector<FeatureVector>> clusters(k);
        for (size_t i = 0; i < features.size(); ++i) {
            double min_dist = 1e9;
            int best_k = 0;
            for (int j = 0; j < k; ++j) {
                double dist = std::sqrt(std::pow(features[i].F1 - centroids[j].F1, 2) + std::pow(features[i].F2 - centroids[j].F2, 2));
                if (dist < min_dist) {
                    min_dist = dist;
                    best_k = j;
                }
            }
            if (assignments[i] != best_k) {
                changed = true;
                assignments[i] = best_k;
            }
            clusters[best_k].push_back(features[i]);
        }
        
        for (int j = 0; j < k; ++j) {
            if (clusters[j].empty()) continue;
            double sum_f1 = 0, sum_f2 = 0;
            for (const auto& f : clusters[j]) {
                sum_f1 += f.F1;
                sum_f2 += f.F2;
            }
            centroids[j].F1 = sum_f1 / clusters[j].size();
            centroids[j].F2 = sum_f2 / clusters[j].size();
        }
    }

    std::ofstream ofs("etalons.txt");
    if (ofs.is_open()) {
        for (int j = 0; j < k; ++j) {
            ofs << std::to_string(j) << " " << centroids[j].F1 << " " << centroids[j].F2 << "\n";
        }
        ofs.close();
        printf("Saved k-means centroids to etalons.txt\n");
    }
}

int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "train") {
        train_kmeans();
    } else if (argc > 1 && std::string(argv[1]) == "test") {
        classify_image();
    } else {
        // By default, do both!
        train_kmeans();
        classify_image();
    }
    return 0;
}
