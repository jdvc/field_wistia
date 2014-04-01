<?php
/**
 * @file
 * Format a wista id
 * Theme functions for the Wistia field module.
 * An associative array containing: 
 * - id: The id of the wistia video
 * - size: the dimensions of the video if custom;
 * - width: the width of the video player
 * - height: the height of the video player
 * - autoplay: API option to autoplay the video
 */

/**
 * Decare new theme_hook function for wistia videos embeds
 * why use array_key_exits?
 */
function theme_field_wistia_video($variables) {
  $id = $variables['video_id'];
  $size = $variables['size'];

  $version = '1.0';
  $height = array_key_exists('height', $variables) ? $variables['height'] : NULL;
  $width =  array_key_exists('width', $variables) ? $variables['width'] : NULL;
  $embedtype = array_key_exists('embed', $variables) ? $variables['embed'] : FALSE;
  $autoplay = array_key_exists('autoplay', $variables) ? $variables['autoplay'] : FALSE;
  $videofoam = array_key_exists('videofoam', $variables) ? $variables['videofoam'] : FALSE;
  $ssl = array_key_exists('ssl', $variables) ? $variables['ssl'] : FALSE;
  $plugin = array_key_exists('plugin', $variables) ? $variables['plugin'] : FALSE;

  // Query Strings based on settings
  $query = array();
  if ($height) {
    $query['height'] = $height;
  }
  if ($width) {
    $query['width'] = $width;
  }
  if ($embedtype) {
    $query['embedType'] = 'api';
  }
  if ($autoplay) {
    $query['autoPlay'] = 'true';
  }
  if ($videofoam) {
    $query['videoFoam'] = 'true';
  }
  if ($ssl) {
    $query['ssl'] = 'true';
  }
  if ($plugin) {
    $query['plugin']['socialbar-v1']['buttons'] = 'twitter-facebook-reddit';
  }

  /* http_build_query Parses an array into a valid, rawurlencoded query string. */
  $url = field_wistia_PUBLIC_EMBED_URL . $id . '?' . http_build_query($query);

  $params = array('url' => $url);

  /* this sends the oembed request to wistia and gets a json array in return
   * The html key in the array holds renderable markup  
   */
  $embed = _media_field_wistia_request_embed($params);

  dpm($embed);
  // Build the oembed URL with options query strings
  $output = $embed['html'];

  return $output;
}

/**
 * Request to embed code from the wistia get embed endpoint.
 * Wistia returns a json array via oembed queury where the html key
 * is the renderable element.
 * @param $query array the quiery string to send to the endpoint.
 * @return array|bool
 */
function _media_field_wistia_request_embed($query) {
  // Make HTTP request.
  $endpoint = variable_get('field_wistia_getembed_url', FIELD_WISTIA_DEFAULT_GETEMBED_URL);
  $result = drupal_http_request(url($endpoint, array('query' => $query)));

  if ($data = json_decode($result->data)) {
    // Success.
    return (array)$data;
  }
  else {
    // Failure. Either the video or playlist doesn't exist on Wistia or there
    // was an error with the request.
    return FALSE;
  }
}

/**
 * Theme function for thumbnails.
 */
function theme_field_wistia_thumbnail($variables) {
  $id = $variables['video_id'];
  $style = $variables['image_style'];

  // Get Wistia settings - TODO is this needed?
  $size = variable_get('field_wistia_size', '420x315');
  $dimensions = field_wistia_get_dimensions($size);

  $files = variable_get('file_public_path', conf_path() . '/files');
  $wistia = variable_get('field_wistia_thumb_dir', 'wistia');
  $dest = $files . '/' . $wistia . '/' . $id . '.png';

  // Check to see if a thumbnail exists locally.
  if (!file_exists($dest)) {
    // Retrieve the image from Wistia.
    if (!field_wistia_get_remote_image($id)) {
      // Use the remote source if local copy fails.
      $src = field_wistia_build_remote_image_path($id);
      return theme('image', array('path' => $src));
    }
  }

  if ($style) {
    $uri = 'public://' . $wistia . '/' . $id . '.png';
    $image = theme('image_style', array('style_name' => $style, 'path' => $uri));
  }
  else {
    $path = $files . '/' . $wistia . '/' . $id . '.png';
    $image = theme('image', array('path' => $path));
  }

  // Check if an url path is provided
  if ($variables['image_link'] != NULL) {
    $url_path = $variables['image_link']['path'];
    $options = $variables['image_link']['options'];
    $image = l($image, $url_path, $options);
  }

  return $image;
}
