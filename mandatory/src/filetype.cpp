/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filetype.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcheel-n <jcheel-n@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:32:16 by eavedill          #+#    #+#             */
/*   Updated: 2024/05/29 00:59:40 by jcheel-n         ###   ########.fr       */
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
	filetypes["mp3"] = "audio/mpeg";
	filetypes["mp4"] = "video/mp4";
	filetypes["mov"] = "video/quicktime";
	filetypes["webm"] = "video/webm";
	filetypes["zip"] = "application/zip";
	filetypes["rar"] = "application/x-rar-compressed";
	filetypes["exe"] = "application/x-msdownload";
	filetypes["msi"] = "application/x-msdownload";
	filetypes["cab"] = "application/vnd.ms-cab-compressed";
	filetypes["doc"] = "application/msword";
	filetypes["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	filetypes["xls"] = "application/vnd.ms-excel";
	filetypes["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	filetypes["ppt"] = "application/vnd.ms-powerpoint";
	filetypes["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	filetypes["7z"] = "application/x-7z-compressed";
	filetypes["iso"] = "application/x-iso9660-image";
	filetypes["pdf"] = "application/pdf";
	return filetypes;
}
