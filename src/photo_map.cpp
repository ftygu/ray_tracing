#include "photo_map.hpp"
#include <queue>

// Implementation details

void Photomap::build_map(const Hittable& world, const Hittable& light, int num_photons) {
    for (int i = 0; i < num_photons; ++i) {
        Photon photon = sample_photon_from_light(light);
        trace_photon(world, photon);
    }
    build_kd_tree();
}

Photon Photomap::sample_photon_from_light(const Hittable& light) {
    // Placeholder for sampling a point on the light source
    // For simplicity, we'll assume the light is a sphere centered at the origin
    Point light_position(0, 2, -2);   // Replace with actual light sampling
    Direction light_normal(0, -1, 0); // Replace with actual normal at light_position
    Color emitted_power(100, 100, 100); // Assume white light for simplicity

    // Generate a random direction in the hemisphere oriented by the light's normal
    Direction photon_direction = random_direction_in_hemisphere(light_normal);

    Photon photon;
    photon.position = light_position;
    photon.direction = photon_direction;
    photon.power = emitted_power;

    return photon;
}

void Photomap::trace_photon(const Hittable& world, Photon& photon) {
    Ray ray(photon.position, photon.direction);

    const int MAX_PHOTON_BOUNCES = 10;
    for (int bounces = 0; bounces < MAX_PHOTON_BOUNCES; ++bounces) {
        HitRecord rec;
        if (world.hit(ray, 0.001, std::numeric_limits<double>::infinity(), rec)) {
            photon.position = rec.p;
            photon.direction = ray.get_direction();

            store_photon(photon);

            ScatterRecord srec;

            rec.material->scatter(ray, rec, srec);

            Color attenuation = srec.attenuation;

            double probability = std::max({ attenuation.r(), attenuation.g(), attenuation.b() });

            if (rand_double() > probability) {
                break;
            }

            photon.power = photon.power * attenuation / 255.0;

            auto r = photon.power.r();
            auto g = photon.power.g();
            auto b = photon.power.b();

            Direction new_direction = random_direction_in_hemisphere(rec.normal);
            ray = Ray(rec.p, new_direction);
        } else {
            break;
        }
    }
}

void Photomap::store_photon(const Photon& photon) {
    photons.push_back(photon);
}

void Photomap::build_kd_tree() {
    root = build_kd_tree_recursive(photons, 0);
}

Photomap::KDNode* Photomap::build_kd_tree_recursive(std::vector<Photon>& photons, int depth) {
    if (photons.empty()) return nullptr;

    int axis = depth % 3;
    size_t median = photons.size() / 2;

    std::nth_element(photons.begin(), photons.begin() + median, photons.end(),
        [axis](const Photon& a, const Photon& b) {
            return a.position[axis] < b.position[axis];
        });

    KDNode* node = new KDNode();
    node->photon = photons[median];
    node->axis = axis;

    std::vector<Photon> left_photons(photons.begin(), photons.begin() + median);
    std::vector<Photon> right_photons(photons.begin() + median + 1, photons.end());

    node->left = build_kd_tree_recursive(left_photons, depth + 1);
    node->right = build_kd_tree_recursive(right_photons, depth + 1);

    return node;
}

std::vector<Photon> Photomap::get_nearby_photons(const Point& p, double radius) const {
    std::vector<Photon> result;
    search_kd_tree(root, p, radius, result);
    return result;
}

std::vector<Photon> Photomap::get_nearby_photons(const Point& p, int num_photons) const {
    std::priority_queue<PhotonDist> result;
    search_kd_tree(root, p, num_photons, result);

    std::vector<Photon> photons;
    while (!result.empty()) {
        photons.push_back(result.top().photon);
        result.pop();
    }
    std::reverse(photons.begin(), photons.end()); // 可选：按距离从近到远排序
    return photons;
}


void Photomap::search_kd_tree(KDNode* node, const Point& p, double radius, std::vector<Photon>& result) const {
    if (!node) return;

    double dist_sq = (node->photon.position - p).length_squared();

    if (dist_sq <= radius * radius) {
        result.push_back(node->photon);
    }

    int axis = node->axis;
    double delta = p[axis] - node->photon.position[axis];

    if (delta < 0) {
        search_kd_tree(node->left, p, radius, result);
        if (delta * delta <= radius * radius) {
            search_kd_tree(node->right, p, radius, result);
        }
    } else {
        search_kd_tree(node->right, p, radius, result);
        if (delta * delta <= radius * radius) {
            search_kd_tree(node->left, p, radius, result);
        }
    }
}

void Photomap::search_kd_tree(KDNode* node, const Point& p, int num_photons, std::priority_queue<PhotonDist>& result) const {
    if (!node) return;

    double dist_sq = (node->photon.position - p).length_squared();

    if (result.size() < num_photons) {
        result.push({dist_sq, node->photon});
    } else if (dist_sq < result.top().dist_sq) {
        result.pop();
        result.push({dist_sq, node->photon});
    }

    int axis = node->axis;
    double delta = p[axis] - node->photon.position[axis];

    KDNode* first = delta < 0 ? node->left : node->right;
    KDNode* second = delta < 0 ? node->right : node->left;

    search_kd_tree(first, p, num_photons, result);

    if (result.size() < num_photons || delta * delta < result.top().dist_sq) {
        search_kd_tree(second, p, num_photons, result);
    }
}


void Photomap::delete_kd_tree(KDNode* node) {
    if (!node) return;
    delete_kd_tree(node->left);
    delete_kd_tree(node->right);
    delete node;
}

// Helper functions

Direction Photomap::random_direction_in_hemisphere(const Direction& normal) const {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double u1 = distribution(generator); 
    double u2 = distribution(generator);

    double r = sqrt(1.0 - u1 * u1);
    double phi = 2 * M_PI * u2;

    Direction dir(r * cos(phi), r * sin(phi), u1);

    // Orthonormal basis
    Direction w = normal;
    Direction a = (fabs(w.x()) > 0.1) ? Direction(0, 1, 0) : Direction(1, 0, 0);
    Direction v = w.cross(a).unit();
    Direction u = v.cross(w);

    // Transform to world coordinates
    Direction direction = u * dir.x() + v * dir.y() + w * dir.z();
    return direction.unit();
}

double Photomap::rand_double() const {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

// Visualization function

void Photomap::visualize_photons(const std::string& filename) const {
    std::clog << "Visualizing photons...\n";
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cerr << "Error: Cannot open file " << filename << " for writing.\n";
        return;
    }

    // Write PLY header
    ofs << "ply\n";
    ofs << "format ascii 1.0\n";
    ofs << "element vertex " << photons.size() << "\n";
    ofs << "property float x\n";
    ofs << "property float y\n";
    ofs << "property float z\n";
    ofs << "property uchar red\n";
    ofs << "property uchar green\n";
    ofs << "property uchar blue\n";
    ofs << "end_header\n";

    // Write photon data
    for (const auto& photon : photons) {

        double r = photon.power.r();
        double g = photon.power.g();
        double b = photon.power.b();

        ofs << photon.position.x() << " "
            << photon.position.y() << " "
            << photon.position.z() << " "
            << static_cast<int>(r) << " "
            << static_cast<int>(g) << " "
            << static_cast<int>(b) << "\n";
    }

    ofs.close();
}
