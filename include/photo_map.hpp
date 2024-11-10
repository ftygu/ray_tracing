#pragma once

#include "basic_types.hpp"
#include "aabb.hpp"
#include "pdf.h"
#include "ray.hpp"
#include "hittable.hpp"
#include "material.hpp"

#include <memory>
#include <queue>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <fstream>
#include <string>
#include <limits>

// Forward declarations
class Hittable;

struct Photon {
    Point position;
    Direction direction;
    Color power;
};

struct PhotonDist {
    double dist_sq;
    Photon photon;

    bool operator<(const PhotonDist& other) const {
        // 最大堆：距离大的优先级高
        return dist_sq < other.dist_sq;
    }
};


class Photomap {
public:
    Photomap() : root(nullptr) {}
    ~Photomap() { delete_kd_tree(root); }

    // Build the photon map by emitting photons from the light source
    void build_map(const Hittable& world, const Hittable& light, int num_photons);

    // Retrieve nearby photons for radiance estimation
    std::vector<Photon> get_nearby_photons(const Point& p, double radius) const;

    std::vector<Photon> get_nearby_photons(const Point& p, int num_photons) const;

    // Visualize all stored photons by exporting to a PLY file
    void visualize_photons(const std::string& filename) const;

    static void visualize_photons(const std::string& filename, std::vector<Photon>& photons) {
        std::clog << "Visualizing photons...\n";
        std::ofstream ofs(filename);
        if (!ofs) {
            std::cerr << "Error: Cannot open file " << filename << " for writing.\n";
            return;
        }

        // Write PLY header
        ofs << "ply\n";
        ofs << "format ascii 1.0\n";
        ofs << "element vertex " << photons.size() << '\n';
        ofs << "property float x\n";
        ofs << "property float y\n";
        ofs << "property float z\n";
        ofs << "property uchar red\n";
        ofs << "property uchar green\n";
        ofs << "property uchar blue\n";
        ofs << "end_header\n";

        // Write photon positions and colors
        for (const Photon& photon : photons) {
            ofs << photon.position.x() << ' ' << photon.position.y() << ' ' << photon.position.z() << ' ';
            ofs << static_cast<int>(photon.power.r()) << ' ' << static_cast<int>(photon.power.g()) << ' ' << static_cast<int>(photon.power.b()) << '\n';
        }
    }

private:
    std::vector<Photon> photons;

    // KD-tree node for efficient photon storage
    struct KDNode {
        Photon photon;
        KDNode* left;
        KDNode* right;
        int axis;

        KDNode() : left(nullptr), right(nullptr), axis(0) {}
    };

    KDNode* root;

    // Methods for KD-tree construction and search
    void build_kd_tree();
    KDNode* build_kd_tree_recursive(std::vector<Photon>& photons, int depth);
    void search_kd_tree(KDNode* node, const Point& p, double radius, std::vector<Photon>& result) const;
    void search_kd_tree(KDNode* node, const Point& p, int num_photons, std::priority_queue<PhotonDist>& result) const;
    void delete_kd_tree(KDNode* node);

    // Photon emission and tracing
    Photon sample_photon_from_light(const Hittable& light);
    void trace_photon(const Hittable& world, Photon& photon);
    void store_photon(const Photon& photon);

    // Helper functions
    Direction random_direction_in_hemisphere(const Direction& normal) const;
    double rand_double() const;

    // Random number generator
    mutable std::default_random_engine generator;
};

