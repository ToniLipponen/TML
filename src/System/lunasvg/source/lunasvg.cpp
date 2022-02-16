#include "lunasvg.h"
#include "layoutcontext.h"
#include "parser.h"

#include <fstream>
#include <cstring>
#include <cmath>

namespace lunasvg {

struct Bitmap::Impl
{
    Impl(std::uint8_t* data, std::uint32_t width, std::uint32_t height, std::uint32_t stride);
    Impl(std::uint32_t width, std::uint32_t height);

    std::unique_ptr<std::uint8_t[]> ownData;
    std::uint8_t* data;
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t stride;
};

Bitmap::Impl::Impl(std::uint8_t* data, std::uint32_t width, std::uint32_t height, std::uint32_t stride)
    : data(data), width(width), height(height), stride(stride)
{
}

Bitmap::Impl::Impl(std::uint32_t width, std::uint32_t height)
    : ownData(new std::uint8_t[width*height*4]), data(nullptr), width(width), height(height), stride(width * 4)
{
}

Bitmap::Bitmap()
{
}

Bitmap::Bitmap(std::uint8_t* data, std::uint32_t width, std::uint32_t height, std::uint32_t stride)
    : m_impl(new Impl(data, width, height, stride))
{
}

Bitmap::Bitmap(std::uint32_t width, std::uint32_t height)
    : m_impl(new Impl(width, height))
{
}

void Bitmap::reset(std::uint8_t* data, std::uint32_t width, std::uint32_t height, std::uint32_t stride)
{
    m_impl.reset(new Impl(data, width, height, stride));
}

void Bitmap::reset(std::uint32_t width, std::uint32_t height)
{
    m_impl.reset(new Impl(width, height));
}

std::uint8_t* Bitmap::data() const
{
    return m_impl ? m_impl->data ? m_impl->data : m_impl->ownData.get() : nullptr;
}

std::uint32_t Bitmap::width() const
{
    return m_impl ? m_impl->width : 0;
}

std::uint32_t Bitmap::height() const
{
    return m_impl ? m_impl->height : 0;
}

std::uint32_t Bitmap::stride() const
{
    return m_impl ? m_impl->stride : 0;
}

bool Bitmap::valid() const
{
    return !!m_impl;
}

Box::Box(double x, double y, double w, double h)
    : x(x), y(y), w(w), h(h)
{}

Box::Box(const Rect& rect)
    : x(rect.x), y(rect.y), w(rect.w), h(rect.h)
{}

Matrix::Matrix(double a, double b, double c, double d, double e, double f)
    : a(a), b(b), c(c), d(d), e(e), f(f)
{}

Matrix::Matrix(const Transform& transform)
    : a(transform.m00), b(transform.m10), c(transform.m01), d(transform.m11), e(transform.m02), f(transform.m12)
{
}

Matrix& Matrix::rotate(double angle)
{
    *this = rotated(angle) * *this;
    return *this;
}

Matrix& Matrix::rotate(double angle, double cx, double cy)
{
    *this = rotated(angle, cx, cy) * *this;
    return *this;
}

Matrix& Matrix::scale(double sx, double sy)
{
    *this = scaled(sx, sy) * *this;
    return *this;
}

Matrix& Matrix::shear(double shx, double shy)
{
    *this = sheared(shx, shy) * *this;
    return *this;
}

Matrix& Matrix::translate(double tx, double ty)
{
   *this = translated(tx, ty) * *this;
    return *this;
}

Matrix& Matrix::transform(double a, double b, double c, double d, double e, double f)
{
    *this = Matrix{a, b, c, d, e, f} * *this;
    return *this;
}

Matrix& Matrix::identity()
{
    *this = Matrix{1, 0, 0, 1, 0, 0};
    return *this;
}

Matrix& Matrix::invert()
{
    *this = inverted();
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& matrix)
{
    *this = *this * matrix;
    return *this; 
}

Matrix& Matrix::premultiply(const Matrix& matrix)
{
    *this = matrix * *this;
    return *this; 
}

Matrix& Matrix::postmultiply(const Matrix& matrix)
{
    *this = *this * matrix;
    return *this; 
}

Matrix Matrix::inverted() const
{
    return Transform(*this).inverted();
}

Matrix Matrix::operator*(const Matrix& matrix) const
{
    return Transform(*this) * Transform(matrix);
}

Box Matrix::map(const Box& box) const
{
    return Transform(*this).map(box);
}

Matrix Matrix::rotated(double angle)
{
    return Transform::rotated(angle);
}

Matrix Matrix::rotated(double angle, double cx, double cy)
{
    return Transform::rotated(angle, cx, cy);
}

Matrix Matrix::scaled(double sx, double sy)
{
    return Transform::scaled(sx, sy);;
}

Matrix Matrix::sheared(double shx, double shy)
{
    return Transform::sheared(shx, shy);
}

Matrix Matrix::translated(double tx, double ty)
{
    return Transform::translated(tx, ty);
}

std::unique_ptr<Document> Document::loadFromFile(const std::string& filename)
{
    std::ifstream fs;
    fs.open(filename);
    if(!fs.is_open())
        return nullptr;

    std::string content;
    std::getline(fs, content, '\0');
    fs.close();

    return loadFromData(content);
}

std::unique_ptr<Document> Document::loadFromData(const std::string& string)
{
    return loadFromData(string.data(), string.size());
}

std::unique_ptr<Document> Document::loadFromData(const char* data, std::size_t size)
{
    ParseDocument parser;
    if(!parser.parse(data, size))
        return nullptr;

    auto root = parser.layout();
    if(!root || root->children.empty())
        return nullptr;

    std::unique_ptr<Document> document(new Document);
    document->root = std::move(root);
    return document;
}

std::unique_ptr<Document> Document::loadFromData(const char* data)
{
    return loadFromData(data, std::strlen(data));
}

Document* Document::rotate(double angle)
{
    root->transform.rotate(angle);
    return this;
}

Document* Document::rotate(double angle, double cx, double cy)
{
    root->transform.rotate(angle, cx, cy);
    return this;
}

Document* Document::scale(double sx, double sy)
{
    root->transform.scale(sx, sy);
    return this;
}

Document* Document::shear(double shx, double shy)
{
    root->transform.shear(shx, shy);
    return this;
}

Document* Document::translate(double tx, double ty)
{
    root->transform.translate(tx, ty);
    return this;
}

Document* Document::transform(double a, double b, double c, double d, double e, double f)
{
    root->transform.transform(a, b, c, d, e, f);
    return this;
}

Document* Document::identity()
{
    root->transform.identity();
    return this;
}

void Document::setMatrix(const Matrix& matrix)
{
    root->transform = Transform(matrix);
}

Matrix Document::matrix() const
{
    return root->transform;
}

Box Document::box() const
{
    return root->map(root->strokeBoundingBox());
}

double Document::width() const
{
    return root->width;
}

double Document::height() const
{
    return root->height;
}

void Document::render(Bitmap bitmap, const Matrix& matrix, std::uint32_t backgroundColor) const
{
    RenderState state(nullptr, RenderMode::Display);
    state.canvas = Canvas::create(bitmap.data(), bitmap.width(), bitmap.height(), bitmap.stride());
    state.transform = Transform(matrix);
    state.canvas->clear(backgroundColor);
    root->render(state);
    state.canvas->rgba();
}

unsigned char* Document::render(std::int32_t& w, std::int32_t& h, std::uint32_t backgroundColor) const
{
    if(root->width == 0.0 || root->height == 0.0)
        return nullptr;

    if(w == 0 && h == 0)
    {
        w = static_cast<std::int32_t>(std::ceil(root->width));
        h = static_cast<std::int32_t>(std::ceil(root->height));
    }
    else if(w != 0 && h == 0)
    {
        h = static_cast<std::int32_t>(std::ceil(w * root->height / root->width));
    }
    else if(h != 0 && w == 0)
    {
        w = static_cast<std::int32_t>(std::ceil(h * root->width / root->height));
    }

    auto* data = new unsigned char[w*h*4];
    Matrix matrix{w / root->width, 0, 0, h / root->height, 0, 0};
    RenderState state(nullptr, RenderMode::Display);
    state.canvas = Canvas::create(data, w, h, w*4);
    state.transform = Transform(matrix);
    state.canvas->clear(backgroundColor);
    root->render(state);
    state.canvas->rgba();
    return data;
}

Bitmap Document::renderToBitmap(std::uint32_t width, std::uint32_t height, std::uint32_t backgroundColor) const
{
    if(root->width == 0.0 || root->height == 0.0)
        return Bitmap{};

    if(width == 0 && height == 0)
    {
        width = static_cast<std::uint32_t>(std::ceil(root->width));
        height = static_cast<std::uint32_t>(std::ceil(root->height));
    }
    else if(width != 0 && height == 0)
    {
        height = static_cast<std::uint32_t>(std::ceil(width * root->height / root->width));
    }
    else if(height != 0 && width == 0)
    {
        width = static_cast<std::uint32_t>(std::ceil(height * root->width / root->height));
    }

    Bitmap bitmap{width, height};
    Matrix matrix{width / root->width, 0, 0, height / root->height, 0, 0};
    render(bitmap, matrix, backgroundColor);
    return bitmap;
}

Document::Document()
{
}

Document::~Document()
{
}

} // namespace lunasvg
