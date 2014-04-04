<?php

/**
 * @file
 *   drush integration for color-thief.
 */

/**
 * The Color Thief plugin URI.
 */
define('COLORTHIEF', 'https://github.com/lokesh/color-thief/archive/v2.0.zip');
define('COLORTHIEF_DOWNLOAD_PREFIX', 'color-thief-');

/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * See `drush topic docs-commands` for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function resp_img_grid_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['color-thief'] = array(
    'callback' => 'drush_resp_img_grid_color_thief_download',
    'description' => dt('Download and install Color Thief.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. A path where to install the Color Thief. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('colorthief'),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function resp_img_grid_drush_help($section) {
  switch ($section) {
    case 'drush:color-thief':
      return dt('Download and install Color Thief from http://lokeshdhakar.com/projects/color-thief, default location is sites/all/libraries.');
  }
}

/**
 * Implements drush_MODULE_post_pm_enable().
 */
// function drush_color-thief_post_pm_enable() {
//   $modules = func_get_args();
//   if (in_array('color-thief', $modules)) {
//     drush_color-thief_plugin();
//   }
// }

/**
 * Command to download the Color Thief plugin.
 */
function drush_resp_img_grid_color_thief_download() {
  $args = func_get_args();
  if (!empty($args[0])) {
    $path = $args[0];
  }
  else {
    $path = 'sites/all/libraries';
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  // Download the zip archive
  if ($filepath = drush_download_file(COLORTHIEF_DOWNLOAD_URI)) {
    $filename = basename($filepath);
    $dirname = COLORTHIEF_DOWNLOAD_PREFIX . basename($filepath, '.zip');

    // Remove any existing Color Thief plugin directory
    if (is_dir($dirname) || is_dir('color-thief')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('color-thief', TRUE);
      drush_log(dt('An existing Color Thief was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive
    drush_tarball_extract($filename);

    // Change the directory name to "color-thief" if needed.
    if ($dirname != 'color-thief') {
      drush_move_dir($dirname, 'color-thief', TRUE);
      $dirname = 'color-thief';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('Color Thief has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install Color Thief to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}
