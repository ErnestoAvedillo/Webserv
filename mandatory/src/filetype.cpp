/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filetype.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavedill <eavedill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:32:16 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/19 17:32:54 by eavedill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <string>

std::map<std::string, std::string> create_filetypes()
{
	std::map<std::string, std::string> filetypes ;

	filetypes["html"] = "text/html";
	filetypes["htm"] = "text/html";
	filetypes["shtml"] = "text/html";
	filetypes["css"] = "text/css";
	filetypes["xml"] = "text/xml";
	filetypes["gif"] = "image/gif";
	filetypes["jpeg"] = "image/jpeg";
	filetypes["jpg"] = "image/jpeg";
	filetypes["js"] = "application/javascript";
	filetypes["atom"] = "application/atom+xml";
	filetypes["rss"] = "application/rss+xml";
	filetypes["mml"] = "text/mathml";
	filetypes["txt"] = "text/plain";
	filetypes["jad"] = "text/vnd.sun.j2me.app-descriptor";
	filetypes["wml"] = "text/vnd.wap.wml";
	filetypes["htc"] = "text/x-component";
	filetypes["avif"] = "image/avif";
	filetypes["png"] = "image/png";
	filetypes["svg"] = "image/svg+xml";
	filetypes["svgz"] = "image/svg+xml";
	filetypes["tif"] = "image/tiff";
	filetypes["tiff"] = "image/tiff";
	filetypes["wbmp"] = "image/vnd.wap.wbmp";
	filetypes["webp"] = "image/webp";
	filetypes["ico"] = "image/x-icon";
	filetypes["jng"] = "image/x-jng";
	filetypes["bmp"] = "image/x-ms-bmp";
	filetypes["woff"] = "font/woff";
	filetypes["woff2"] = "font/woff2";
	filetypes["jar"] = "application/java-archive";
	filetypes["war"] = "application/java-archive";
	filetypes["ear"] = "application/java-archive";
	filetypes["json"] = "application/json";
	filetypes["hqx"] = "application/mac-binhex40";
	filetypes["doc"] = "application/msword";
	filetypes["pdf"] = "application/pdf";
	filetypes["ps"] = "application/postscript";
	filetypes["eps"] = "application/postscript";
	filetypes["ai"] = "application/postscript";
	filetypes["rtf"] = "application/rtf";
	filetypes["m3u8"] = "application/vnd.apple.mpegurl";
	filetypes["kml"] = "application/vnd.google-earth.kml+xml";
	filetypes["kmz"] = "application/vnd.google-earth.kmz";
	filetypes["xls"] = "application/vnd.ms-excel";
	filetypes["eot"] = "application/vnd.ms-fontobject";
	filetypes["ppt"] = "application/vnd.ms-powerpoint";
	filetypes["odg"] = "application/vnd.oasis.opendocument.graphics";
	filetypes["odp"] = "application/vnd.oasis.opendocument.presentation";
	filetypes["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	filetypes["odt"] = "application/vnd.oasis.opendocument.text";
	filetypes["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	filetypes["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	filetypes["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	filetypes["wmlc"] = "application/vnd.wap.wmlc";
	filetypes["wasm"] = "application/wasm";
	filetypes["7z"] = "application/x-7z-compressed";
	filetypes["cco"] = "application/x-cocoa";
	filetypes["jardiff"] = "application/x-java-archive-diff";
	filetypes["jnlp"] = "application/x-java-jnlp-file";
	filetypes["run"] = "application/x-makeself";
	filetypes["pl"] = "application/x-perl";
	filetypes["pm"] = "application/x-perl";
	filetypes["prc"] = "application/x-pilot";
	filetypes["pdb"] = "application/x-pilot";
	filetypes["rar"] = "application/x-rar-compressed";
	filetypes["rpm"] = "application/x-redhat-package-manager";
	filetypes["sea"] = "application/x-sea";
	filetypes["swf"] = "application/x-shockwave-flash";
	filetypes["sit"] = "application/x-stuffit";
	filetypes["tcl"] = "application/x-tcl";
	filetypes["tk"] = "application/x-tcl";
	filetypes["der"] = "application/x-x509-ca-cert";
	filetypes["pem"] = "application/x-x509-ca-cert";
	filetypes["crt"] = "application/x-x509-ca-cert";
	filetypes["xpi"] = "application/x-xpinstall";
	filetypes["xhtml"] = "application/xhtml+xml";
	filetypes["xspf"] = "application/xspf+xml";
	filetypes["zip"] = "application/zip";
	filetypes["bin"] = "application/octet-stream";
	filetypes["exe"] = "application/octet-stream";
	filetypes["dll"] = "application/octet-stream";
	filetypes["deb"] = "application/octet-stream";
	filetypes["dmg"] = "application/octet-stream";
	filetypes["iso"] = "application/octet-stream";
	filetypes["img"] = "application/octet-stream";
	filetypes["msi"] = "application/octet-stream";
	filetypes["msp"] = "application/octet-stream";
	filetypes["msm"] = "application/octet-stream";
	filetypes["mid"] = "audio/midi";
	filetypes["midi"] = "audio/midi";
	filetypes["kar"] = "audio/midi";
	filetypes["mp3"] = "audio/mpeg";
	filetypes["ogg"] = "audio/ogg";
	filetypes["m4a"] = "audio/x-m4a";
	filetypes["ra"] = "audio/x-realaudio";
	filetypes["3gpp"] = "video/3gpp";
	filetypes["3gp"] = "video/3gpp";
	filetypes["ts"] = "video/mp2t";
	filetypes["mp4"] = "video/mp4";
	filetypes["mpeg"] = "video/mpeg";
	filetypes["mpg"] = "video/mpeg";
	filetypes["mov"] = "video/quicktime";
	filetypes["webm"] = "video/webm";
	filetypes["flv"] = "video/x-flv";
	filetypes["m4v"] = "video/x-m4v";
	filetypes["mng"] = "video/x-mng";
	filetypes["asx"] = "video/x-ms-asf";
	filetypes["asf"] = "video/x-ms-asf";
	filetypes["wmv"] = "video/x-ms-wmv";
	filetypes["avi"] = "video/x-msvideo";

	// Executables
	
	filetypes["bin"] = "application/octet-stream";
	filetypes["elf"] = "application/x-executable";
	filetypes["jar"] = "application/java-archive";
	filetypes["pl"] = "application/x-perl";
	filetypes["py"] = "text/x-python";
	filetypes["rb"] = "application/x-ruby";
	filetypes["sh"] = "application/x-sh";
	filetypes["js"] = "application/javascript";
	filetypes["bat"] = "application/bat";
	filetypes["ps1"] = "application/x-powershell";
	filetypes["avi"] = "video/x-msvideo";
	filetypes["php"] = "application/x-httpd-php";
	filetypes["c"] = "text/x-c";
	filetypes["cpp"] = "text/x-c++";
	filetypes["java"] = "text/x-java-source";
	filetypes["cs"] = "text/x-csharp";
	filetypes["swift"] = "text/x-swift";
	filetypes["go"] = "text/x-go";
	filetypes["rust"] = "text/rust";
	filetypes["dart"] = "application/dart";
	filetypes["kotlin"] = "text/x-kotlin";
	return filetypes;
}
