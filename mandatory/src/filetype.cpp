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
	return filetypes;
}
