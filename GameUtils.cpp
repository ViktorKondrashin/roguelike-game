#include "GameUtils.h"

bool Collision::PixelPerfectTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2, sf::Uint8 alphaThreshold) {
  if (!BoundingBoxTest(sprite1, sprite2))
    return false;
  sf::FloatRect bounds1 = sprite1.getGlobalBounds();
  sf::FloatRect bounds2 = sprite2.getGlobalBounds();
  sf::IntRect overlap;
  overlap.left = std::max(bounds1.left, bounds2.left);
  overlap.top = std::max(bounds1.top, bounds2.top);
  overlap.width = std::min(bounds1.left + bounds1.width, bounds2.left + bounds2.width) - overlap.left;
  overlap.height = std::min(bounds1.top + bounds1.height, bounds2.top + bounds2.height) - overlap.top;

  const sf::Texture* tex1 = sprite1.getTexture();
  const sf::Texture* tex2 = sprite2.getTexture();
  sf::Image img1 = tex1->copyToImage();
  sf::Image img2 = tex2->copyToImage();

  for (int y = overlap.top; y < overlap.top + overlap.height; ++y) {
    for (int x = overlap.left; x < overlap.left + overlap.width; ++x) {

      sf::Vector2f pos1 = sprite1.getInverseTransform().transformPoint(x, y);
      sf::Vector2f pos2 = sprite2.getInverseTransform().transformPoint(x, y);

      if (pos1.x >= 0 && pos1.y >= 0 && pos1.x < img1.getSize().x && pos1.y < img1.getSize().y &&
        pos2.x >= 0 && pos2.y >= 0 && pos2.x < img2.getSize().x && pos2.y < img2.getSize().y) {

        if (img1.getPixel(pos1.x, pos1.y).a > alphaThreshold &&
          img2.getPixel(pos2.x, pos2.y).a > alphaThreshold) {
          return true;
        }
      }
    }
  }
  return false;
}
