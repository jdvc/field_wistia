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
function theme_wistia_video($variables) {
  $id = $variables['video_id'];
  $size = $variables['size'];

  $version = '1.0';
  $height = array_key_exists('height', $variables) ? $variables['height'] : NULL;
  $width =  array_key_exists('width', $variables) ? $variables['width'] : NULL;
  $embedtype = array_key_exists('embed', $variables) ? $variables['embed'] : NULL;
  $autoplay = array_key_exists('autoplay', $variables) ? $variables['autoplay'] : FALSE;
  $videofoam = array_key_exists('videofoam', $variables) ? $variables['videofoam'] : FALSE;
  $ssl = array_key_exists('ssl', $variables) ? $variables['ssl'] : FALSE;

  // Query Strings based on settings
  $query = array();
  if ($height) {
    $query['height'] = $height;
  }
  if ($width) {
    $query['width'] = $height;
  }
  if ($embedtype) {
    $query['embedType'] = $embedtype;
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

  /* http_build_query Parses an array into a valid, rawurlencoded query string. */
  $url = MEDIA_WISTIA_PUBLIC_EMBED_URL . $id . '?' . http_build_query($query);

  $params = array('url' => $url);

  /* this sends the oembed request to wistia and gets a json array in return
   * The html key in the array holds renderable markup  
   */
  $embed = _media_wistia_request_embed($params);

  // Build the oembed URL with options query strings
  $output = $embed['html'];

  return $output;
  dpm($output);
  dpm($query);
}

/**
 * Request to embed code from the wistia get embed endpoint.
 * Wistia returns a json array via oembed queury where the html key
 * is the renderable element.
 * @param $query array the quiery string to send to the endpoint.
 * @return array|bool
 */
function _media_wistia_request_embed($query) {
  // Make HTTP request.
  $endpoint = variable_get('media_wistia_getembed_url', MEDIA_WISTIA_DEFAULT_GETEMBED_URL);
  $result = drupal_http_request(url($endpoint, array('query' => $query)));

  dpm($result);

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
