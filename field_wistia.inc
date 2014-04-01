<?php

/**
 * @ file
 * Wistia field helper functions.
 */

/**
 * Returns a list of standard Wistia video sizes.
 */
function field_wistia_field_size_options() {
  return array(
    '480x360' => '480px by 360px (4:3)',
    '640x360' => '640px by 360px (16:9)',
    '960x540' => '960px by 540px (16:9)',
    'custom' => 'custom',
  );
}

/* 
 * regex provided from wistia docs to parse
 * their URLs
 */

function field_wistia_field_get_video_id($input) {
  $patterns = array(
    '/https?:\/\/(.+)?(wistia\.com|wi\.st)\/(medias|embed)\/(.*)/',
    '/https?:\/\/(.+)?(wistia\.net|wi\.st)\/(medias|embed)\/.*\/([^"]+)/',
  );
  foreach ($patterns as $pattern) {
    preg_match($pattern, $input, $matches);
    if (isset($matches[4])) {
      $video_id = $matches[4];

      return $video_id;
    }
  }  
}


/**
 * Splits height and width when given size, as from field_wistia_size_options.
 */
function field_wistia_field_get_dimensions($size = NULL, $width = NULL, $height = NULL) {
  $dimensions = array();
  if ($size == 'custom') {
    $dimensions['width'] = (int) $width;
    $dimensions['height'] = (int) $height;
  }
  else {
    // Locate the 'x'.
    $strpos = strpos($size, 'x');
    // Width is the first dimension.
    $dimensions['width'] = substr($size, 0, $strpos);
    // Height is the second dimension.
    $dimensions['height'] = substr($size, $strpos + 1, strlen($size));
  }

  return $dimensions;
}

/**
 * Retrieve wistia thumbnail image via Wistia API.
 *
 * TODO add error messaging if something goes wrong, and return FALSE.
 */
function field_wistia_field_get_remote_image($id = NULL) {
  $url = FIELD_WISTIA_FIELD_PUBLIC_EMBED_URL . $id;

  $params = array('url' => $url);
  $embed = _media_field_wistia_request_embed($params);

  $image_result = $embed['thumbnail_url'];
  dpm($image_result);

  // Assure the wistia thumbnail directory exists.
  $files = variable_get('file_public_path', conf_path() . '/files');
  $field_wistia_dir = variable_get('field_wistia_field_thumb_dir', 'wistia');
  $field_wistia_path = $files . '/' . $field_wistia_dir;
  if (!file_prepare_directory($field_wistia_path, FILE_CREATE_DIRECTORY) && !mkdir($field_wistia_path, 0775, TRUE)) {
    watchdog('wistia', 'Failed to create Wistia thumbnail directory: %dir', array('%dir' => $field_wistia_path), WATCHDOG_ERROR);
  }

  // Save the file.
  $dest = $files . '/' . $field_wistia_dir . '/' . $id . '.png';
  file_put_contents($dest, $image_result);

  return TRUE;
}

/**
 * Get images by building correctly formed URL - kinda hackey.
 */
function field_wistia_field_build_remote_image_path($id = NULL) {
  if (!$id) {
    return;
  }
  // Default image thumbnail.
  $default = 'http://img.wistia.com/vi/' . $id . '/default.jpg'; 

  return url($default);
}
